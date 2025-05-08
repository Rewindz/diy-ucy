
#include "calcs.h"

void normalize_nic_base(NicBase* base) {
    float nonNic = 100.0f - base->nicPercent;
    base->realPgPercent = base->nomPgPercent * (nonNic / 100.0f);
    base->realVgPercent = base->nomVgPercent * (nonNic / 100.0f);
}

void compute_nicotine_volume(Target* target, float batchSize) {
    target->nic.volume = batchSize * (target->nicPercent / 100.0f);
}

void compute_nic_base_volume(NicBase* base, float requiredNicVolume) {
    if (base->nicPercent == 0.0f)
        base->totalVolume = 0.0f;
    else
        base->totalVolume = requiredNicVolume / (base->nicPercent / 100.0f);
}

void split_nic_base_into_components(NicBase* base) {
    base->nic.volume = base->totalVolume * (base->nicPercent / 100.0f);
    base->pg.volume = base->totalVolume * (base->realPgPercent / 100.0f);
    base->vg.volume = base->totalVolume * (base->realVgPercent / 100.0f);
}

void compute_target_pg_vg(Target* target, float batchSize, float nicVol) {
    float remaining = batchSize - nicVol;
    target->pg.volume = remaining * (target->nomPgPercent / 100.0f);
    target->vg.volume = remaining * (target->nomVgPercent / 100.0f);
}

void determine_additional_pg_vg(MixAdd* add, Target* target, NicBase* base) {
    add->addPg.volume = target->pg.volume - base->pg.volume;
    add->addVg.volume = target->vg.volume - base->vg.volume;
}

void handle_flavors(MixAdd* add, float batchSize) {
    for (int i = 0; i < add->flavorCount; ++i) {
        Flavor* f = &add->flavors[i];
        f->volume = batchSize * (f->percent / 100.0f);

        if (f->isVG)
            add->addVg.volume -= f->volume;
        else
            add->addPg.volume -= f->volume;
    }
}

void calculate_totals(NicBase* base) {
    base->totalVolume = base->nic.volume + base->pg.volume + base->vg.volume;
    base->totalWeight = base->nic.weight + base->pg.weight + base->vg.weight;

    if (base->totalVolume == 0.0f)
        base->density = 0.0f;
    else
        base->density = base->totalWeight / base->totalVolume;
}

void calculate_mix(MixData* mix) {
    normalize_nic_base(&mix->nicBase);

    compute_nicotine_volume(&mix->target, mix->batchSize);
    compute_nic_base_volume(&mix->nicBase, mix->target.nic.volume);

    split_nic_base_into_components(&mix->nicBase);
    compute_target_pg_vg(&mix->target, mix->batchSize, mix->nicBase.nic.volume);

    determine_additional_pg_vg(&mix->mixAdd, &mix->target, &mix->nicBase);

    if (mix->mixAdd.flavorCount > 0)
        handle_flavors(&mix->mixAdd, mix->batchSize);

    calculate_totals(&mix->nicBase);
    calculate_totals((NicBase*)&mix->target); // Casting for convenience
}

MixData create_mixdata(MixInputs *input)
{
    MixData mix = {0};

    mix.batchSize = input->batchSize;

    // Nicotine base
    mix.nicBase.nicPercent = input->baseNicPercent;
    mix.nicBase.nomPgPercent = input->basePgPercent;
    mix.nicBase.nomVgPercent = 100.0f - input->basePgPercent;

    // Target
    mix.target.nicPercent = input->targetNicPercent;
    mix.target.nomPgPercent = input->targetPgPercent;
    mix.target.nomVgPercent = 100.0f - input->targetPgPercent;

    // Flavors
    if (input->flavorCount > 0 && input->flavorCount <= MAX_FLAVORS && input->flavors != NULL) {
        mix.mixAdd.flavorCount = input->flavorCount;
        memcpy(mix.mixAdd.flavors, input->flavors, input->flavorCount * sizeof(Flavor));
    }

    // Zero out additions (handled later in calc)
    mix.mixAdd.addPg.volume = 0.0f;
    mix.mixAdd.addVg.volume = 0.0f;

    return mix;
}

float mgml_to_percent(float mgml, float density) {
    return (mgml / (density * 1000.0f)) * 100.0f;
}
