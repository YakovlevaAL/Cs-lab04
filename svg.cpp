#include "svg.h"
#include "histogram.h"

void svg_begin(double width, double height)
{
    cout << "<?xml version='1.0' encoding='UTF-8'?>\n";
    cout << "<svg ";
    cout << "width='" << width << "' ";
    cout << "height='" << height << "' ";
    cout << "viewBox='0 0 " << width << " " << height << "' ";
    cout << "xmlns='http://www.w3.org/2000/svg'>\n";
}

void svg_end()
{
    cout << "</svg>\n";
}

void svg_text(double left, double baseline, string text)
{
    cout << "<text x='" << left << "' y='" << baseline << "'>" << text << "</text>";
}

void svg_rect(double x, double y, double width, double height, string stroke,
              string fill)
{
    cout << "<rect x='" << x << "' y='" << y << "' width='" << width
         << "' height='" << height << "' stroke='" << stroke
         << "' fill='" << fill << "'/>";
}

string
make_info_text() {
    stringstream buffer;

    DWORD info = GetVersion();
    DWORD mask = 0x0000ffff;
    DWORD mask_2 = 0x000000ff;
    DWORD platform = info >> 16;
    DWORD version = info & mask;
    if ((info & 0x80000000) == 0)
    {
        DWORD version_major = version & mask_2;
        DWORD version_minor = version >> 8;
        DWORD build = platform;
        buffer << "Windows v" << version_major << "." << version_minor << " (build " << build << ") ";
    }

    DWORD size = MAX_COMPUTERNAME_LENGTH+1;
    char computer_name[size];
    GetComputerNameA(computer_name, &size);
    buffer << "Computer name: " << computer_name;

    return buffer.str();
}

void show_histogram_svg(const vector<size_t>& bins)
{
    const auto IMAGE_WIDTH = 500;
    const auto IMAGE_HEIGHT = 300;
    const auto TEXT_LEFT = 20;
    const auto TEXT_BASELINE = 20;
    const auto TEXT_WIDTH = 50;
    const auto BIN_HEIGHT = 30;
    const auto BLOCK_WIDTH = 10;
    const size_t SCREEN_WIDTH = 80;
    const size_t MAX_ASTERISK = SCREEN_WIDTH - 4 - 1;

    svg_begin(IMAGE_WIDTH, IMAGE_HEIGHT);

    double top = 0;

    size_t max_count = 0;
    for (size_t count : bins)
        {
        if (count > max_count)
            max_count = count;
        }

        const bool scaling_needed = max_count > MAX_ASTERISK;

        double sr = mean(bins);

        if (scaling_needed)
        {
            const double scaling_factor = (double)MAX_ASTERISK / max_count;

            for (size_t bin : bins)
            {
                    auto  height = (size_t)(bin * scaling_factor);
                    const double bin_width = BLOCK_WIDTH * height;
                    svg_text(TEXT_LEFT, top + TEXT_BASELINE, to_string(bin));
                    if ((double)bin <= sr)
                    {
                        svg_rect(TEXT_WIDTH, top, bin_width, BIN_HEIGHT, "green",
                                 "#ffeeee");
                    }
                    else
                    {
                        svg_rect(TEXT_WIDTH, top, bin_width, BIN_HEIGHT, "red",
                                 "#ffeeee");
                    }
                            top += BIN_HEIGHT;
                    }
        }
        else
        {
            for (size_t bin : bins)
            {
                    const double bin_width = BLOCK_WIDTH * bin;
                    svg_text(TEXT_LEFT, top + TEXT_BASELINE, to_string(bin));
                    if ((double)bin <= sr)
                    {
                        svg_rect(TEXT_WIDTH, top, bin_width, BIN_HEIGHT, "green",
                                 "#ffeeee");
                    }
                    else
                    {
                        svg_rect(TEXT_WIDTH, top, bin_width, BIN_HEIGHT, "red",
                                 "#ffeeee");
                    }
                    top += BIN_HEIGHT;
            }
        }
    svg_text(0, top + TEXT_BASELINE, make_info_text());
    svg_end();
}
