TARGET=laserScan
all: $(TARGET)

CFLAGS=-fPIC -g -Wall 
ARIA_INCLUDE=-I/usr/local/Aria/include 
ARIA_LINK=-L/usr/local/Aria/lib -lAria -lpthread -ldl -lrt 

%: %.cpp
	$(CXX) $(CFLAGS) $(ARIA_INCLUDE) $< -o $@ $(ARIA_LINK)
	scp $@ pip.goucher.edu:$@

clean:
	rm $(TARGET)	