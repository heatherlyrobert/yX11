#================================[[ beg-code ]]================================#



#===(manditory)============================================#
# basename of executable, header, and all code files
NAME_BASE  = yX11
# additional standard and outside libraries
LIB_STD    = -lX11 -lGL -lm
# all heatherly libraries used, debug versions whenever available
LIB_MINE   = -lySTR_debug -lyREGEX_debug
# directory for production code, no trailing slash
INST_DIR   = 



#===(optional)=============================================#
# extra include directories required
INC_MINE   = 
# utilities generated, separate from main program
NAME_UTIL  = 
# libraries only for the utilities
LIB_UTIL   = 



#===(post-processing)======================================#
# create a rule for...
#install_prep       :
#remove_prep        :



#===(master template)======================================#
include /usr/local/sbin/make_program



#===(post-processing)======================================#
# create a rule for...
#install_post       :
#remove_post        :



#================================[[ end-code ]]================================#
#
##*============================----beg-of-source---============================*#
#
#
#
##*---(current variables)--------------*#
#BASE    = yX11
#
##*---(standard variables)-------------*#
#COMPG   = gcc -c -std=c99 -g -pg -Wall -Wextra
#COMP    = tcc -g -Wall -Wunsupported -Wwrite-strings -bench
#AS_LIB  = -shared -soname lib${BASE}.so.1  -o lib${BASE}.so.1.0
#INCS    =
#
#LINK    = gcc
#LIBS    = -L/usr/local/lib -L/lib64 -L/usr/lib64 -L/usr/lib64/opengl/nvidia/lib  -lX11 -lGL -lm
#
#COPY    = cp -f
#CLEAN   = rm -f
#PRINT   = @printf
#
#gcc                : ${BASE}.h ${BASE}.c
#	${COMPG} -fPIC ${INCS} ${BASE}.c
#	${LINK}  -shared -Wl,-soname,lib${BASE}.so.1  ${LIBS}  -o lib${BASE}.so.1.0  ${BASE}.o
#
##tcc                : ${BASE}.h ${BASE}.c
##	${PRINT}  "\n--------------------------------------\n"
##	${PRINT}  "compile ${BASE} using tcc\n"
##	${COMP}   ${AS_LIB}  ${LIBS}  ${BASE}.c
#
#
##*---(utilities)----------------------*#
#clean              :
#	${PRINT}  "\n--------------------------------------\n"
#	${PRINT}  "cleaning out local object, backup, and temp files\n"
#	${CLEAN}  *.o
#	${CLEAN}  *~
#	${CLEAN}  temp*
#
#bigclean           :
#	${PRINT}  "\n--------------------------------------\n"
#	${PRINT}  "clean out local swap files\n"
#	${CLEAN} '.'*.sw?
#
#install            :
#	${PRINT}  "\n--------------------------------------\n"
#	${PRINT}  "install ${BASE} into production\n"
#	@sha1sum   lib${BASE}.so.1.0
#	_lib      -s  ${BASE}
#	ldconfig
#
#remove             :
#	${PRINT}  "\n--------------------------------------\n"
#	${PRINT}  "remove ${BASE} from production\n"
#	_lib      -d  ${BASE}
#	ldconfig
#
#
##*============================----end-of-source---============================*#
