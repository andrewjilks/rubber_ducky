.PHONY: build flash sync clean

build:
	idf.py build && make sync

flash:
	idf.py flash

monitor:
	idf.py monitor

clean:
	idf.py clean

sync:
	git add .
	git commit -m "Auto-sync: $(shell date '+%Y-%m-%d %H:%M:%S')"
	git push origin main
