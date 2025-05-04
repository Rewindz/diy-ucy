#ifndef CALCS_H_
#define CALCS_H_

#include <string.h>

#include "constants.h"

#define MAX_FLAVORS 10

typedef struct {
    float volume;
    float weight;
    float density;
} Ingredient;

typedef struct {
    float nicPercent;        // e.g., 2.7%
    float nomPgPercent;      // e.g., 70%
    float nomVgPercent;      // 100 - nomPgPercent
    float realPgPercent;
    float realVgPercent;
    float totalVolume;
    float totalWeight;
    float density;
    Ingredient nic;
    Ingredient pg;
    Ingredient vg;
} NicBase;

typedef struct {
    float nicPercent;
    float nomPgPercent;
    float nomVgPercent;
    float totalVolume;
    float totalWeight;
    float density;
    Ingredient nic;
    Ingredient pg;
    Ingredient vg;
} Target;

typedef struct {
    float percent;   // flavor percentage of batch
    int isVG;        // 1 = VG based, 0 = PG based
    float volume;
} Flavor;

typedef struct {
    int flavorCount;
    Flavor flavors[MAX_FLAVORS];
    Ingredient addPg;
    Ingredient addVg;
} MixAdd;

typedef struct {
    float batchSize;     // total mix size in mL
    NicBase nicBase;
    Target target;
    MixAdd mixAdd;
} MixData;

typedef struct{
  float batchSize;
  float baseNicPercent;
  float basePgPercent;
  float targetNicPercent;
  float targetPgPercent;
  int flavorCount;
  Flavor *flavors;
} MixInputs;

MixData create_mixdata(float batchSize,
                       float baseNicPercent,
                       float basePgPercent,
                       float targetNicPercent,
                       float targetPgPercent,
                       int flavorCount,
                       const Flavor* flavors);


// Calculation function declarations
void normalize_nic_base(NicBase* base);
void compute_nicotine_volume(Target* target, float batchSize);
void compute_nic_base_volume(NicBase* base, float requiredNicVolume);
void split_nic_base_into_components(NicBase* base);
void compute_target_pg_vg(Target* target, float batchSize, float nicVol);
void determine_additional_pg_vg(MixAdd* add, Target* target, NicBase* base);
void handle_flavors(MixAdd* add, float batchSize);
void calculate_totals(NicBase* base);  // can reuse for target too

float mgml_to_percent(float mgml, float density);

// Master calculation function
void calculate_mix(MixData* mix);

#endif //CALCS_H_

