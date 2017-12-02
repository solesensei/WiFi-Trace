#include "postprocessing.h"

using std::tuple;

Image postprocessing(Image src)
{
    Image res = autocontrast(src, 0.01);
    return res;
}

tuple<uint,uint,uint> overflow_crop(const tuple<uint,uint,uint>& pix)
{   
    int r,g,b;
    std::tie(r,g,b) = pix;

    r = r < 0 ? 0 : (r > 255 ? 255 : r);
    g = g < 0 ? 0 : (g > 255 ? 255 : g); 
    b = b < 0 ? 0 : (b > 255 ? 255 : b); 
                 
    return std::make_tuple(r,g,b);

}

Image autocontrast(Image src_image, double fraction)
{    
    double Ymin = 255;
    double Ymax = 0;
    double r_cl,g_cl,b_cl;
    for (uint i = 0; i < src_image.n_rows; ++i)
        for (uint j = 0; j < src_image.n_cols; ++j)
        {
            std::tie(r_cl,g_cl,b_cl)  = src_image(i,j);
            const double Y = 0.2125 * r_cl + 0.7154 * g_cl + 0.0721 * b_cl;
            Ymax = Y > Ymax ? Y : Ymax;
            Ymin = Y < Ymin ? Y : Ymin;            
        }
        Ymax = Ymax - (Ymax-Ymin)*fraction;
        Ymin = Ymin + (Ymax-Ymin)*fraction;
        assert(Ymax > Ymin);
    for (uint i = 0; i < src_image.n_rows; ++i)
        for (uint j = 0; j < src_image.n_cols; ++j)
        {
            std::tie(r_cl,g_cl,b_cl)  = src_image(i,j);
            double fr = (r_cl - Ymin) * 255 / (Ymax - Ymin);
            double fg = (g_cl - Ymin) * 255 / (Ymax - Ymin);
            double fb = (b_cl - Ymin) * 255 / (Ymax - Ymin);
        
            src_image(i,j) = overflow_crop(std::make_tuple(fr, fg, fb));
        }
    return src_image;
}