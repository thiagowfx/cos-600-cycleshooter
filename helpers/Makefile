all: solution1

solution1:
	cp 51-cycleshooter-udev.rules /etc/udev/rules.d/51-cycleshooter-udev.rules

solution2:
	gpasswd -a ${USER} dialout # uucp

clean:
	$(RM) /etc/udev/rules.d/51-cycleshooter-udev.rules

.PHONY: all clean solution1 solution2
