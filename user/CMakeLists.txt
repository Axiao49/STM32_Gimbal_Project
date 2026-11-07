cmake_minimum_required(VERSION 3.10.0)
project(user VERSION 0.1.0 LANGUAGES C)

add_executable(user main.c stm32f10x_it.c system_stm32f10x.c sys_state.c)

