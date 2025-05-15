#include "callbacks.h"

void load_btn_cb(void *screen)
{
  menu_item_t *file_item
    = create_new_menu_item(ITEM_TYPE_INPUT,
			   "Save File Name");
  draw_input_popup(file_item);
    
  free_whole_screen((menu_screen_t *) screen);
  *app_screen = load_app_from_file(file_item->value);

  free_menu_item(file_item);
}

void save_btn_cb(void *screen)
{
}

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
  sprintf(menu_batch->items[4]->value, "%%");
}

void by_weight_cb(void *screen)
{
  nic_by_weight = 1;
  sprintf(menu_batch->items[4]->value, "mg/ml");
}

void flavour_input_cb(void *screen)
{
  menu_item_t *item = get_selected_item((menu_screen_t *)screen);

  //hack
  menu_item_t *temp_item = create_new_menu_item(ITEM_TYPE_LABEL, "Flavour Name");
  if(strcmp("Flavour", item->label)){
    free(temp_item->value);
    temp_item->value = strdup(item->label);
  }
  draw_input_popup(temp_item);

  //could get away with not freeing here and just transfering
  // the ptr but its easier this way lol
  free(item->label);
  item->label = strdup(temp_item->value);
  free_menu_item(temp_item);
  clear_and_draw((menu_screen_t *)screen);
}

void add_flavour_cb(void *screen)
{
  menu_item_t *item
    = create_new_menu_item(ITEM_TYPE_INPUT,
			   "Flavour");
  add_item_callback(item, flavour_input_cb);
  menu_add_item(menu_flavours, item);
}

void remove_flavour_cb(void *screen)
{
  if(menu_flavours->item_count <= 2)
    return;
  menu_remove_item(menu_flavours, menu_flavours->item_count - 1);
  clear_and_draw((menu_screen_t *)screen);
}

/**
   TODO:
   Need to add the nic base from the calculation
   Save and Load
   Vol and weight in results
   Print the units in the results
   UX?
 **/
void submit_recipe_cb(void *screen)
{
  int flavour_count = menu_flavours->item_count - 2;
  if(flavour_count <= 0)
    return;
  
  { // Get data from user inputs
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
    sprintf(buffer, "%s: %.2f", menu_flavours->items[i + 2]->label,
	    volume_to_mass(mix_data.mixAdd.flavors[i].volume, DENS_FLAVOUR));
    menu_item_t *item
      = create_new_menu_item(ITEM_TYPE_LABEL,
			     buffer);
    menu_add_item(new_menu, item);
  }

  {
    char buffer[256];

    sprintf(buffer, "Add Nic: %.2f", volume_to_mass(mix_data.nicBase.totalVolume, DENS_NIC));
    //sprintf(buffer, "Add Nic: %.2f", mix_data.nicBase.totalVolume);
    menu_item_t *add_nic
      = create_new_menu_item(ITEM_TYPE_LABEL,
			     buffer);
    sprintf(buffer, "Add PG: %.2f", mix_data.mixAdd.addPg.volume);
    menu_item_t *add_pg
      = create_new_menu_item(ITEM_TYPE_LABEL,
			     buffer);
    sprintf(buffer, "Add VG: %.2f", mix_data.mixAdd.addVg.volume);
    menu_item_t *add_vg
      = create_new_menu_item(ITEM_TYPE_LABEL,
			     buffer);
    menu_add_item(new_menu, add_nic);
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
