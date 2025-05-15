project('diy-ucy', 'c')

files = ['main.c', 'render.c', 'input.c', 'constants.c', 'calcs.c', 'menus/appmenu.c', 'menus/save.c', 'menus/callbacks.c']

ncursesdep = dependency('ncurses')

typesincl = include_directories('./menus/')
typeslib = static_library('types', './menus/types.c')
typesdep = declare_dependency(link_with : typeslib, include_directories : typesincl)



executable('diyucy', files, c_args : ['-Wall'],  dependencies : [ncursesdep, typesdep])
