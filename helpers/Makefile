all:
	@getent group dialout &>/dev/null && gpasswd -a $(shell logname) dialout || true
	@getent group uucp &>/dev/null && gpasswd -a $(shell logname) uucp || true

clean:
	@getent group dialout &>/dev/null && gpasswd -d $(shell logname) dialout || true
	@getent group uucp &>/dev/null && gpasswd -d $(shell logname) uucp || true

.PHONY: all clean
