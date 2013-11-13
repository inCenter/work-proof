#ifndef PRODUCTIVITYINDICATOR_H
#define PRODUCTIVITYINDICATOR_H

#include <QString>

class ProductivityIndicator
{
    int m_productivitySeconds   = 0;
    int m_productivePercent     = 0;

public:
    ProductivityIndicator() = default;
    ProductivityIndicator(int prod_seconds, int prod_percent) {
        m_productivePercent     = prod_percent;
        m_productivitySeconds   = prod_seconds;
    }

    QString getLabel();
};

#endif // PRODUCTIVITYINDICATOR_H
