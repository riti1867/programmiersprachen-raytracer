#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "color.hpp"
#include "pixel.hpp"
#include "window.hpp"
#include "ppmwriter.hpp"
#include "camera.hpp"
#include "shape.hpp"
#include "light.hpp"
#include "composite.hpp"

#include <string>
#include <memory>
#include <glm/glm.hpp>

enum RenderingFlags {

  ANTIALIASING = 0x01,
  RECURSION = 0x02,
  FRAMES = 0x04,
  SOURCE = 0x08,
  HELP = 0x10

};

class Renderer {

  public:

    std::shared_ptr<Camera> cam_;
    std::shared_ptr<Shape> root_;
    std::shared_ptr<std::vector<std::shared_ptr<Light>>> lights_;
    std::vector<Color> color_buffer_;
    Color background_;

    std::string output_directory_;
    std::string full_path_;

    int initial_recursion_limit = 4;

  private:

    PpmWriter ppm_;

    std::string filename_;

    unsigned int width_;
    unsigned int height_;

    int recursion_limit;

  public:

    Renderer() :
      output_directory_("output"), ppm_(600u, 600u), color_buffer_(600u * 600u, Color{0.0f, 0.0f, 0.0f}), width_(600u), height_(600u) {}

    Renderer(std::string const& file, std::shared_ptr<Camera> cam, unsigned int w, unsigned int h) : 
      output_directory_("output"), ppm_(w, h), cam_(cam), filename_(file), color_buffer_(w * h, Color{0.0f, 0.0f, 0.0f}), width_(w), height_(h) {}

    bool is_valid();
    void render(int flags);
    void write(Pixel const& p);

    // getter
    inline std::vector<Color> const& get_color_buffer() const {return color_buffer_;}
    inline std::string const get_filename() const {return filename_;}
    inline unsigned int const get_width() const {return width_;}
    inline unsigned int const get_height() const {return height_;}

  private:

    Color trace(Ray const& ray);
    void tone_mapping(Color& color);
    Color gamma_correction(Color& color);
    Hitpoint find_intersection(Ray const& ray);
    
    Color calc_ambient_color(Hitpoint const& hitpoint, std::shared_ptr<Shape> shape);
    Color calc_diffuse_color(Hitpoint const& hitpoint, std::shared_ptr<Shape> shape);
    Color calc_specular_color(Hitpoint const& hitpoint, std::shared_ptr<Shape> shape);
    
};

#endif // RENDERER_HPP
