#include "appmenu.h"


MixInputs mix_inputs = {0};
MixData mix_data = {0};
int nic_by_weight = 0;
menu_screen_t **app_screen;
menu_t *menu_submit;
menu_t *menu_flavours;
menu_t *menu_batch;
menu_t *menu_target;
menu_t *menu_base;

menu_screen_t **create_app_menu()
{
  
  menu_screen_t **screen_ptr = calloc(1, sizeof(menu_screen_t **));
  if(!screen_ptr)
    return NULL;
  
  menu_screen_t *screen;
  { //Create sections
    section_t *section_batch;
    section_t *section_nic_base;
    section_t *section_target;
    section_t *section_flavours;
    section_t *section_recipe;
    section_t *section_save;


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

      menu_item_t *current_selection
	= create_new_menu_item(ITEM_TYPE_LABEL_VALUED,
			       "Selection");
      sprintf(current_selection->value, "%%");
      menu_t *menu
	= create_new_menu(5, target_vol, str_lbl, by_vol, by_weight, current_selection);
    
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

    { // Load & Save Section

      menu_item_t *load_btn
	= create_new_menu_item(ITEM_TYPE_BUTTON,
			       "Load");
      add_item_callback(load_btn, load_btn_cb);

      menu_item_t *save_btn
	= create_new_menu_item(ITEM_TYPE_BUTTON,
			       "Save");
      add_item_callback(save_btn, save_btn_cb);

      menu_t *menu
	= create_new_menu(2, load_btn, save_btn);
      
      section_save = create_new_section("Load & Save", menu);
      
    }

    screen = create_new_menu_screen(6,
				    section_batch,
				    section_nic_base,
				    section_target,
				    section_flavours,
				    section_recipe,
				    section_save
				    );
    
  }

  *screen_ptr = screen;
  app_screen = screen_ptr;
  return screen_ptr;
}

