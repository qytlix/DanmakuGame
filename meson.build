#
# main
#
project('DanmakuGame', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14'])


SDL2_dep = dependency('sdl2',)
SDL2_image_dep = dependency('sdl2_image')

executable('DanmakuGame',
           'main.cpp',
           dependencies : [SDL2_dep, SDL2_image_dep],
           install : true)
