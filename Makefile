CC=g++
CCFLAGS= -g -Wall --std=c++11
SRC:= $(wildcard  *.cpp )
OBJS:=$(patsubst %.cpp,%.o,$(SRC))
INCLDPATH=-I.
AR:=ar rc
LIB_STATIC_OBJECT = ./libmyBase.a
LIB_DYNAMIC_OBJECT = ./libmyBase.so

.PHONY:all
all:$(LIB_DYNAMIC_OBJECT)
$(LIB_DYNAMIC_OBJECT):$(OBJS)
	$(CC) -shared -o $(LIB_DYNAMIC_OBJECT) $(OBJS)
%.o : %.cpp  
	$(CC) -fPIC  -c $(CCFLAGS) $(INCLDPATH) -o $@ $<


static:$(LIB_STATIC_OBJECT)
$(LIB_STATIC_OBJECT):$(OBJS)
	$(AR)   $(LIB_STATIC_OBJECT) $(OBJS)
# %.o : %.cpp  
# 	$(CC) -c $(CCFLAGS) $(INCLDPATH) -o $@ $<

.PHONY:install
install:
	cp $(LIB_DYNAMIC_OBJECT) ../Run-Project/so/
	cp $(LIB_DYNAMIC_OBJECT) ../clearing-cpp/
	cp $(LIB_DYNAMIC_OBJECT) ../matching-service-cpp/

.PHONY:debug
debug:
	sudo cp $(LIB_DYNAMIC_OBJECT) /usr/lib/

clean:
	# rm  -rf $(OBJS)
	rm *.o
