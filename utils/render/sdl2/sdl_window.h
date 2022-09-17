#pragma once
#include <SDL.h>
#include <memory>
#include <string>
#include <stdint.h>
#include <vector>

// Taken from SO, this is another case where we need a custom destructor since C is a fan
// of incomplete types..
struct SDLWindowDestroyer {
    void operator()(SDL_Window* w) const {
    SDL_DestroyWindow(w);
    }
};

struct SDLSurfaceDestroyer {
    void operator()(SDL_Surface* w) const {
      //The window takes care of this...
    }
    void operator()(SDL_Texture* w) const {
      //The window takes care of this...
    }
    void operator()(SDL_Renderer* w) const {
      //The window takes care of this...
    }
};

namespace snickers {

    struct RGB {
      uint8_t r = 0x00;
      uint8_t g = 0xff;
      uint8_t b = 0x00;
      uint8_t a = 0xFF;
      operator uint32_t() const {
        return 0xFF000000 | (uint32_t) r | (uint32_t)(g) << 8 | (uint32_t)(b) << 16;
      }
      auto at(int index) const -> uint8_t  {
        switch(index) {
          break; case 0: return a;
          break; case 1: return r;
          break; case 2: return g;
          break; case 3: return b;
        }
        return (uint8_t)0;
      }
    };
    struct SdlWindow {
      SdlWindow(const std::string name, const size_t width, const size_t height);

      auto createWindow() -> void;

      auto fillWithColour(const RGB colour) -> void;
      auto setColour(const size_t x, const size_t y, const RGB colour) -> void;
      auto renderPixelMap() -> void;

      private:
        const size_t screenWidth_;
        const size_t screenHeight_;
        const std::string name_;
        std::unique_ptr<SDL_Window, SDLWindowDestroyer> window_;
        std::unique_ptr<SDL_Surface, SDLSurfaceDestroyer> surface_; // Not sure I really need this
        std::unique_ptr<SDL_Texture, SDLSurfaceDestroyer> texture_;
        std::unique_ptr<SDL_Renderer, SDLSurfaceDestroyer> renderer_;
        std::vector<uint32_t> pixelMap_;
        uint32_t pitch_;
    };
};
