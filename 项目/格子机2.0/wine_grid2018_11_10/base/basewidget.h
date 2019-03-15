#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>

class BaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseWidget(QWidget *parent = 0);

signals:

public slots:
    
    /*
    void Blur1D(bitmap_t& bitmap, filter_t& kernel)
        {
            Normalization(kernel);

            buffer_t buff(bitmap);

            for (long inx = 0, y = 0; y < bitmap.h(); ++y)
            {
                for (long x = 0; x < bitmap.w(); ++x, ++inx)
                {
                    for (long n = 0, i = -kernel.radius(); i <= kernel.radius(); ++i, ++n)
                    {
                        long i_k = Edge(i, x, bitmap.w());
                        long inx_k = inx + i_k;
                        buff[inx].r += bitmap[inx_k].r * kernel[n];
                        buff[inx].g += bitmap[inx_k].g * kernel[n];
                        buff[inx].b += bitmap[inx_k].b * kernel[n];
                    }
                }
            }

            for (long inx = 0, x = 0; x < bitmap.w(); ++x)
            {
                for (long y = 0; y < bitmap.h(); ++y)
                {
                    inx = y * bitmap.w() + x;
                    double r = 0.0, g = 0.0, b = 0.0;
                    for (long n = 0, i = -kernel.radius(); i <= kernel.radius(); ++i, ++n)
                    {
                        long i_k = Edge(i, y, bitmap.h());
                        long inx_k = inx + i_k * bitmap.w();
                        r += buff[inx_k].r * kernel[n];
                        g += buff[inx_k].g * kernel[n];
                        b += buff[inx_k].b * kernel[n];
                    }
                    bitmap[inx].r = Clamp<bitmap_t::channel_t>(r);
                    bitmap[inx].g = Clamp<bitmap_t::channel_t>(g);
                    bitmap[inx].b = Clamp<bitmap_t::channel_t>(b);
                }
            }
        }
*/
};

#endif // BASEWIDGET_H
