#include "appmenu.h"







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
      menu_item_t *by_weight
	= create_new_menu_item(ITEM_TYPE_BUTTON,
			       "Weight (mg/ml)");
      menu_t *menu
	= create_new_menu(4, target_vol, str_lbl, by_vol, by_weight);
      
      section_batch = create_new_section("Batch", menu);
    }

    { // Nicotine Base Section
      menu_item_t *strength
	= create_new_menu_item(ITEM_TYPE_INPUT,
			       "Strength (% OR mg/ml)");
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
			       "Strength (% OR mg/ml)");
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

      menu_item_t *dummy
	= create_new_menu_item(ITEM_TYPE_LABEL,
			       "Placeholder");
      menu_t *menu
	= create_new_menu(1, dummy);

      section_flavours = create_new_section("Flavours", menu);

    }

    { // Recipe Output Section
      
      menu_item_t *dummy
	= create_new_menu_item(ITEM_TYPE_LABEL,
			       "Placeholder");
      menu_t *menu
	= create_new_menu(1, dummy);

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

