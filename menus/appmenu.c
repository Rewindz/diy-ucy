#include "appmenu.h"


MixInputs mix_inputs = {0};
MixData mix_data = {0};
int nic_by_weight = 0;
menu_t *menu_submit;
menu_t *menu_flavours;
menu_t *menu_batch;
menu_t *menu_target;
menu_t *menu_base;


menu_t *create_results_menu()
{
  
  return NULL; //todo
}


// Callback Handlers

void base_pg_cb(void *screen)
{
  float pg;
  sscanf(menu_base->items[2]->value, "%f", &pg);
  char buffer[256];
  sprintf(buffer, "%.2f", 100.0 - pg);
  free(menu_base->items[1]->value);
  menu_base->items[1]->value = strdup(buffer);
}

void base_vg_cb(void *screen)
{
  float vg;
  sscanf(menu_base->items[1]->value, "%f", &vg);
  char buffer[256];
  sprintf(buffer, "%.2f", 100.0 - vg);
  free(menu_base->items[2]->value);
  menu_base->items[2]->value = strdup(buffer);
}

void target_pg_cb(void *screen)
{
  float pg;
  sscanf(menu_target->items[2]->value, "%f", &pg);
  char buffer[256];
  sprintf(buffer, "%.2f", 100.0 - pg);
  free(menu_target->items[1]->value);
  menu_target->items[1]->value = strdup(buffer);
}

void target_vg_cb(void *screen)
{
  float vg;
  sscanf(menu_target->items[1]->value, "%f", &vg);
  char buffer[256];
  sprintf(buffer, "%.2f", 100.0 - vg);
  free(menu_target->items[2]->value);
  menu_target->items[2]->value = strdup(buffer);
}

void by_vol_cb(void *screen)
{
  nic_by_weight = 0;
}

void by_weight_cb(void *screen)
{
  nic_by_weight = 1;
}

void add_flavour_cb(void *screen)
{
  menu_item_t *item
    = create_new_menu_item(ITEM_TYPE_INPUT,
			   "Flavour");
  menu_add_item(menu_flavours, item);
}

void remove_flavour_cb(void *screen)
{
  if(menu_flavours->item_count <= 2)
    return;
  menu_remove_item(menu_flavours, menu_flavours->item_count - 1);
  clear_and_draw((menu_screen_t *)screen);
}

void submit_recipe_cb(void *screen)
{
  int flavour_count = menu_flavours->item_count - 2;
  if(flavour_count <= 0)
    return;

  {
    sscanf(menu_batch->items[0]->value, "%f", &mix_inputs.batchSize);
    float base_percent = 0.0f;
    sscanf(menu_base->items[0]->value, "%f", &base_percent);
    mix_inputs.baseNicPercent = (nic_by_weight ? mgml_to_percent(base_percent, DENS_NIC) : base_percent);
    
    float base_vg, base_pg;
    sscanf(menu_base->items[1]->value, "%f", &base_vg);
    sscanf(menu_base->items[2]->value, "%f", &base_pg);
    mix_inputs.basePgPercent = 100.0 - base_vg;

    float target_percent = 0.0f;
    sscanf(menu_target->items[0]->value, "%f", &target_percent);
    mix_inputs.targetNicPercent = (nic_by_weight ? mgml_to_percent(target_percent, DENS_NIC) : target_percent);

    float target_vg, target_pg;
    sscanf(menu_target->items[1]->value, "%f", &target_vg);
    sscanf(menu_target->items[2]->value, "%f", &target_pg);
    mix_inputs.targetPgPercent = 100.0 - target_vg;
    
  }
  
  menu_t *new_menu = create_new_blank_menu();

  mix_inputs.flavorCount = flavour_count;
  Flavor *flavours_arr = calloc(flavour_count, sizeof(Flavor));
  
  for(int i = 0; i < flavour_count; ++i){
    float temp = 0;
    sscanf(menu_flavours->items[i + 2]->value, "%f", &temp);
    flavours_arr[i].percent = temp;
    flavours_arr[i].isVG = 0;
  }

  mix_inputs.flavors = flavours_arr;

  mix_data = create_mixdata(&mix_inputs);
  calculate_mix(&mix_data);

  for(int i = 0; i < mix_data.mixAdd.flavorCount; ++i){
    char buffer[256];
    sprintf(buffer, "%.2f", mix_data.mixAdd.flavors[i].volume);
    menu_item_t *item
      = create_new_menu_item(ITEM_TYPE_LABEL,
			     buffer);
    menu_add_item(new_menu, item);
  }

  {
    char buffer[256];
    sprintf(buffer, "Add PG: %.2f", mix_data.mixAdd.addPg.volume);
    menu_item_t *add_pg
      = create_new_menu_item(ITEM_TYPE_LABEL,
			     buffer);
    sprintf(buffer, "Add VG: %.2f", mix_data.mixAdd.addVg.volume);
    menu_item_t *add_vg
      = create_new_menu_item(ITEM_TYPE_LABEL,
			     buffer);
    menu_add_item(new_menu, add_pg);
    menu_add_item(new_menu, add_vg);
  }

  menu_item_t *submit_btn
    = create_new_menu_item(ITEM_TYPE_BUTTON,
			   "Submit");
  add_item_callback(submit_btn, submit_recipe_cb);
  menu_add_item(new_menu, submit_btn);
  
  replace_menu((menu_screen_t *)screen, menu_submit, new_menu);

  menu_submit = new_menu;

  //mix_data.mixAdd.flavors = NULL;
  free(flavours_arr);

  clear_and_draw((menu_screen_t *)screen);

}




menu_screen_t *create_app_menu()
{
  
  menu_screen_t *screen;
  { //Create sections
    section_t *section_batch;
    section_t *section_nic_base;
    section_t *section_target;
    section_t *section_flavours;
    section_t *section_recipe;


    { // Batch Section
      menu_item_t *target_vol
	= create_new_menu_item(ITEM_TYPE_INPUT,
			       "Target in ml");
      menu_item_t *str_lbl
	= create_new_menu_item(ITEM_TYPE_LABEL,
			       "Nicotine strength in:");
      menu_item_t *by_vol
	= create_new_menu_item(ITEM_TYPE_BUTTON,
			       "Volume (%)");
      add_item_callback(by_vol, by_vol_cb);
      menu_item_t *by_weight
	= create_new_menu_item(ITEM_TYPE_BUTTON,
			       "Weight (mg/ml)");
      add_item_callback(by_weight, by_weight_cb);
      menu_t *menu
	= create_new_menu(4, target_vol, str_lbl, by_vol, by_weight);

      menu_batch = menu;
      
      section_batch = create_new_section("Batch", menu);
    }

    { // Nicotine Base Section
      menu_item_t *strength
	= create_new_menu_item(ITEM_TYPE_INPUT,
			       "Strength");
      menu_item_t *vg
	= create_new_menu_item(ITEM_TYPE_INPUT,
			       "VG %");
      add_item_callback(vg, base_vg_cb);
      menu_item_t *pg
	= create_new_menu_item(ITEM_TYPE_INPUT,
			       "PG %");
      add_item_callback(pg, base_pg_cb);
      menu_t *menu
	= create_new_menu(3, strength, vg, pg);

      menu_base = menu;
      
      section_nic_base = create_new_section("Nic Base", menu);

    }

    { // Target Nicotine Section
      menu_item_t *strength
	= create_new_menu_item(ITEM_TYPE_INPUT,
			       "Strength");
      menu_item_t *vg
	= create_new_menu_item(ITEM_TYPE_INPUT,
			       "VG %");
      add_item_callback(vg, target_vg_cb);
      menu_item_t *pg
	= create_new_menu_item(ITEM_TYPE_INPUT,
			       "PG %");
      add_item_callback(pg, target_pg_cb);
      menu_t *menu
	= create_new_menu(3, strength, vg, pg);

      menu_target = menu;
      
      section_target = create_new_section("Target", menu);
      
    }

    { // Flavours Section

      menu_item_t *add_f
	= create_new_menu_item(ITEM_TYPE_BUTTON,
			       "Add Flavour");
      add_item_callback(add_f, add_flavour_cb);

      menu_item_t *remove_f
	= create_new_menu_item(ITEM_TYPE_BUTTON,
			       "Remove Flavour");
      add_item_callback(remove_f, remove_flavour_cb);
      
      menu_t *menu
	= create_new_menu(2, add_f, remove_f);

      menu_flavours = menu;

      section_flavours = create_new_section("Flavours", menu);

    }

    { // Recipe Output Section
      
      menu_item_t *submit
	= create_new_menu_item(ITEM_TYPE_BUTTON,
			       "Submit");
      add_item_callback(submit, submit_recipe_cb);
      
      menu_t *menu
	= create_new_menu(1, submit);

      menu_submit = menu;
      
      section_recipe = create_new_section("Recipe", menu);
      
    }

    screen = create_new_menu_screen(5,
				    section_batch,
				    section_nic_base,
				    section_target,
				    section_flavours,
				    section_recipe
				    );
    
  }

  return screen;
}

