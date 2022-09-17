#include "sdl_window.h"
#include <iostream>

namespace snickers {

  SdlWindow::SdlWindow(const std::string name, const size_t width, const size_t height) : name_(name), screenWidth_(width), screenHeight_(height) {}
  auto SdlWindow::createWindow() -> void {
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
      std::cout << "something messed up\n";
      return;
    }


    window_.reset(SDL_CreateWindow(name_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth_, screenHeight_, 0));
    renderer_.reset(SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_TARGETTEXTURE));
    surface_.reset(SDL_GetWindowSurface(window_.get()));
    texture_.reset(SDL_CreateTexture(renderer_.get(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, screenWidth_, screenHeight_));

    pixelMap_.resize(screenWidth_*screenHeight_);
    for(int i =0; i < screenWidth_*screenHeight_; i++) {
      pixelMap_[i] = 0;
    }
  }

  auto SdlWindow::fillWithColour(const RGB colour) -> void {
    SDL_FillRect(surface_.get(), NULL, SDL_MapRGB(surface_->format, colour.r,colour.g,colour.b));
    SDL_UpdateWindowSurface(window_.get());
  }

  auto SdlWindow::setColour(const size_t x, const size_t y, const RGB colour) -> void {
      pixelMap_[x+(y*screenWidth_)] = (uint32_t)colour;
  }

  auto SdlWindow::renderPixelMap() -> void {
    uint32_t* start = &pixelMap_[0];
    SDL_UpdateTexture(texture_.get(), nullptr, start, screenWidth_*sizeof(uint32_t));
    SDL_RenderCopy(renderer_.get(), texture_.get(), nullptr, nullptr);
    SDL_RenderPresent(renderer_.get());
    SDL_PumpEvents();
  }
};
