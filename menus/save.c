#include "save.h"



AppMenu_t *l_new_menu()
{
  AppMenu_t *new_appmenu = calloc(1, sizeof(AppMenu_t));
  if(!new_appmenu) return NULL;
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
      
      section_flavours = create_new_section("Flavours", menu);

    }

    { // Recipe Output Section
      
      menu_item_t *submit
	= create_new_menu_item(ITEM_TYPE_BUTTON,
			       "Submit");
      add_item_callback(submit, submit_recipe_cb);
      
      menu_t *menu
	= create_new_menu(1, submit);
          
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

    new_appmenu->screen = screen;
    new_appmenu->section_batch = section_batch;
    new_appmenu->section_nic_base = section_nic_base;
    new_appmenu->section_target = section_target;
    new_appmenu->section_flavours = section_flavours;
    new_appmenu->section_recipe = section_recipe;
    new_appmenu->section_save = section_save;
  }
    
  return new_appmenu;
}

/**
   TODO:
   Base type


 **/
int parse_save_file(AppMenu_t *menu, const char *save_file)
{
  FILE *stream;
  char *line = NULL;
  size_t size = 0;
  ssize_t nread;
  stream = fopen(save_file, "r");
  if(!stream)
    return -1;

  while ( (nread = getline(&line, &size, stream)) != -1) {

    {
      char *last_char = (line + strlen(line) - 1);
      if(*last_char == '\n')
	*last_char = '\0';
    }
    
    int cmd_len = strcspn(line, ":");
    char *cmd = strndup(line, cmd_len);
    char *val = strchr(line, ':') + 1;

    menu_item_t *item = NULL;
    
    if(!strcmp(cmd, "NIC_STR"))
      item = menu->section_nic_base->menu->items[0];
    else if(!strcmp(cmd, "NIC_VG"))
      item = menu->section_nic_base->menu->items[1];
    else if(!strcmp(cmd, "NIC_PG"))
      item = menu->section_nic_base->menu->items[2];
    else if(!strcmp(cmd, "TGT_STR"))
      item = menu->section_target->menu->items[0];
    else if(!strcmp(cmd, "TGT_VG"))
      item = menu->section_target->menu->items[1];
    else if(!strcmp(cmd, "TGT_PG"))
      item = menu->section_target->menu->items[2];
    else if(!strcmp(cmd, "BA_ML"))
      item = menu->section_batch->menu->items[0];

    else if(!strcmp(cmd, "STR_SEL")){
      if(!strcmp(val, "%")){
	nic_by_weight = 0;
	sprintf(menu->section_batch->menu->items[4]->value, "%%");
      }else if(!strcmp(val, "MG")){
	nic_by_weight = 1;
	sprintf(menu->section_batch->menu->items[4]->value, "mg/ml");
      }
    }
    else if(!strcmp(cmd, "FLA")){
      int flv_name_len = strcspn(val, "_");
      char *flv_name = strndup(val, flv_name_len);
      char *flv_amt = strchr(val, '_') + 1;
      menu_item_t *flv_item
	= create_new_menu_item(ITEM_TYPE_INPUT, flv_name);
      free(flv_item->value);
      flv_item->value = strdup(flv_amt);
      add_item_callback(flv_item, flavour_input_cb);
      menu_add_item(menu->section_flavours->menu, flv_item);
      free(flv_name);
    }
    
    if(item){
      free(item->value);
      item->value = strdup(val);
    }

    free(cmd);
   
  }

  free(line);
  fclose(stream);

  return 0;
}

void save_app_to_file(AppMenu_t *menu, const char *save_file)
{
  if(!menu || !save_file)
    return;
  FILE *stream = fopen(save_file, "w+");
  if(!stream)
    return;

  
  //Probably not safe and bug ridden
  char *flavours = calloc(1024, sizeof(char));
  menu_t *fla_menu = menu->section_flavours->menu;
  for(int i = 2; i < fla_menu->item_count; ++i){
    char buf[256];
    sprintf(buf, "FLA:%s_%s\n", fla_menu->items[i]->label, fla_menu->items[i]->value);
    strcat(flavours, buf);
  }
  
  fprintf(stream,
	  "NIC_STR:%s\n"
	  "NIC_VG:%s\n"
	  "NIC_PG:%s\n"
	  "TGT_STR:%s\n"
	  "TGT_VG:%s\n"
	  "TGT_PG:%s\n"
	  "BA_ML:%s\n"
	  "%s",
	  menu->section_nic_base->menu->items[0]->value,
	  menu->section_nic_base->menu->items[1]->value,
	  menu->section_nic_base->menu->items[2]->value,
	  menu->section_target->menu->items[0]->value,
	  menu->section_target->menu->items[1]->value,
	  menu->section_target->menu->items[2]->value,
	  menu->section_batch->menu->items[0]->value,
	  flavours
	  );

  free(flavours);
  fclose(stream);
  
}

//man getline(3)
menu_screen_t *load_app_from_file(const char *save_file)
{
  AppMenu_t *new_menu = l_new_menu();
  if(!new_menu)
    abort();

  if(parse_save_file(new_menu, save_file) == -1)
    abort(); //Change plz
  
  menu_screen_t *ret = new_menu->screen;
  free(new_menu);
  return ret;
}
