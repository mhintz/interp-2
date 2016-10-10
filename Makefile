.PHONY: build run

all: build run

build:
	xcodebuild -configuration Debug -project xcode/reaction_diffusion.xcodeproj/

run:
	./xcode/build/Debug/reaction_diffusion.app/Contents/MacOS/reaction_diffusion
