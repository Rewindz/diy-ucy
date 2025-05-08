#include "appmenu.h"


MixInputs mix_inputs;
MixData mix_data;
menu_t *menu_submit;
menu_t *menu_flavours;



menu_t *create_results_menu()
{
  
  return NULL; //todo
}


// Callback Handlers

void by_vol_cb(void *screen)
{
  
}

void by_weight_cb(void *screen)
{

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
  if(flavour_count <= 2)
    return;
  
  //temp
  mix_inputs.batchSize = 120.0f;
  mix_inputs.baseNicPercent = mgml_to_percent(20.0, DENS_NIC);
  mix_inputs.basePgPercent = 0.0f;
  mix_inputs.targetNicPercent = mgml_to_percent(3.0, DENS_NIC);
  mix_inputs.targetPgPercent = 40.0f;
  mix_inputs.flavorCount = flavour_count;
  
  menu_t *new_menu = create_new_blank_menu();

  Flavor *flavours_arr = calloc(flavour_count, sizeof(Flavor));
  
  for(int i = 0; i < flavour_count; ++i){
    
  }
  
  replace_menu((menu_screen_t *)screen, menu_submit, new_menu);

  menu_submit = new_menu;

}




menu_screen_t *create_app_menu()
{

  MixInputs mix_inputs = {0};
  
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
      
      section_batch = create_new_section("Batch", menu);
    }

    { // Nicotine Base Section
      menu_item_t *strength
	= create_new_menu_item(ITEM_TYPE_INPUT,
			       "Strength");
      menu_item_t *vg
	= create_new_menu_item(ITEM_TYPE_INPUT,
			       "VG %");
      menu_item_t *pg
	= create_new_menu_item(ITEM_TYPE_INPUT,
			       "PG %");
      menu_t *menu
	= create_new_menu(3, strength, vg, pg);

      section_nic_base = create_new_section("Nic Base", menu);

    }

    { // Target Nicotine Section
      menu_item_t *strength
	= create_new_menu_item(ITEM_TYPE_INPUT,
			       "Strength");
      menu_item_t *vg
	= create_new_menu_item(ITEM_TYPE_INPUT,
			       "VG %");
      menu_item_t *pg
	= create_new_menu_item(ITEM_TYPE_INPUT,
			       "PG %");
      menu_t *menu
	= create_new_menu(3, strength, vg, pg);

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

