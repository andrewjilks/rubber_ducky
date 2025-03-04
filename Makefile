PROJECT_NAME = rubber_ducky
ESP_DIR = esp_firmware

# ESP-IDF Commands
IDF = idf.py
BUILD = $(IDF) -C $(ESP_DIR) build
FLASH = $(IDF) -C $(ESP_DIR) flash monitor

# Git Commands
GIT_ADD = git add .
GIT_COMMIT = git commit -m "Auto-update: Built and flashed successfully"
GIT_PUSH = git push origin main

# Log the build timestamp
BUILD_TIMESTAMP = $(shell date '+%Y-%m-%d %H:%M:%S')

# Targets
all: build flash update_git

build:
	@echo "🔧 Building firmware..."
	$(BUILD)

flash: build
	@echo "⚡ Flashing firmware..."
	$(FLASH)

update_git: flash
	@echo "✅ Build verified. Updating Git..."
	@echo "Last successful build: $(BUILD_TIMESTAMP)" >> README.md
	$(GIT_ADD)
	$(GIT_COMMIT)
	$(GIT_PUSH)

clean:
	@echo "🧹 Cleaning build files..."
	$(IDF) -C $(ESP_DIR) fullclean

help:
	@echo "Available commands:"
	@echo "  make build      - Build the firmware"
	@echo "  make flash      - Flash the firmware"
	@echo "  make update_git - Push updates after successful build & flash"
	@echo "  make clean      - Remove build files"

