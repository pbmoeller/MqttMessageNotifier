// MqttMessageNotifier
#include "mqtt_tab_style.hpp"

// Qt
#include <QStyleOptionTab>

namespace mmn {

QSize MqttTabStyle::sizeFromContents(QStyle::ContentsType type,
                                     const QStyleOption *option,
                                     const QSize &size,
                                     const QWidget *widget) const
{
    QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
    if(type == QStyle::CT_TabBarTab) {
        s.transpose();
    }
    return s;
}

void MqttTabStyle::drawControl(QStyle::ControlElement element,
                               const QStyleOption *option,
                               QPainter *painter,
                               const QWidget *widget) const
{
    if(element == CE_TabBarTabLabel) {
        if(const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
            QStyleOptionTab opt(*tab);
            opt.shape = QTabBar::RoundedNorth;
            QProxyStyle::drawControl(element, &opt, painter, widget);
            return;
        }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

} // namespace mmn
