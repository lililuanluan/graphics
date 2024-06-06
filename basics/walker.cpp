#include <tuple>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>


// 用 tuple 存储rgb，用 tie 一次性提取出rgb的值
void tuple_rgb() {
    std::tuple rgb = std::make_tuple(212, 24, 118);

    // 有两种方法获取rgb，第一种用 get，根据下标获取，第二种用tie，类似结构化绑定
    int r, g, b;

    r = std::get<0>(rgb);
    std::cout << r << std::endl;

    std::tie(r, g, b) = rgb;
    std::cout << r << ", " << g << ", " << b << std::endl;
}

// 将 rgb 画到图片上
// idx = x + width * y
// 左上角是原点，横轴x，纵轴y
int get_index(int x, int y, int width) {
    return x + width * y;
}

// 图片大小
int calc_size(int width, int height) {
    return width * height;
}

// 将图片存储为 ppm，像素用tuple存，值为0-1之间的浮点数，表示深度值
// ppm: portable pixmap，在unix系统中常用，因为是使用ascii（type=p3）存储信息，所以可移植性好
// 格式
// P3       # 表示是RGB颜色的图片，ASCII存储
// 3 2      # 3行2列
// 255      # 每个通道颜色的最大值
// 255 0 0  # 后面就是具体的像素
// ...
void save_image(std::string_view filename, const std::vector<std::tuple<float, float, float>>& image, int width, int height) {
    std::ofstream out(filename.data());
    out << "P3\n" << width << " " << height << "\n" << "255" << "\n";
    for (auto [r, g, b] : image) {
        out << int(r * 255.0f) << " ";
        out << int(g * 255.0f) << " ";
        out << int(b * 255.0f) << "\n";
    }

}

auto make_gradient(int width, int height) {
    std::vector<std::tuple<float, float, float>> image(width * height);
    int cnt = 0;
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            image[cnt++] = std::make_tuple(float(x) / width, float(y) / height, 0.2);
        }
    }

    return image;
}

auto make_image(int width, int height, std::tuple<float, float, float> color = std::make_tuple(0.0f, 0.0f, 1.0 / 3)) {
    return std::vector<std::tuple<float, float, float>>(width * height, color);
}

void save_image() {
    int width = 640;
    int height = 360;

    auto image = make_image(width, height);
    save_image("../image/blue.ppm", image, width, height);

    // 渐变色（gradient）
    image = make_gradient(width, height);
    save_image("../image/gradient.ppm", image, width, height);
}



void walker() {
    int width = 640;
    int height = 360;
    
    auto mask = make_image(width, height);
    // walker 类似于一个橡皮擦的功能，走过的地方将mask擦除，露出底色，底色是渐变色
    auto back = make_gradient(width, height);


    int x = width / 2;
    int y = height / 2;

    auto walk_from = [&](int x, int y) {for (size_t i = 0; i < 1 << 15; i++) {
        x += rand() % 3 - 1;    // -1 0 1
        y += rand() % 3 - 1;
        // If the value of v is within [lo, hi], returns v; otherwise returns the nearest boundary
        auto idx = std::clamp<int>(get_index(x, y, width), 0, mask.size() - 1); // 超出范围则取边界值
        mask[idx] = back[idx];
    }};

    walk_from(width / 4, height / 2);
    walk_from(width / 2, height / 4);

    save_image("../image/walker.ppm", mask, width, height);

}


int main() {
    // tuple_rgb();
    save_image();
    walker();
}