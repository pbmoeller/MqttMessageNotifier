#ifndef MMN_UI_MQTT_TAB_STYLE_HPP
#define MMN_UI_MQTT_TAB_STYLE_HPP

#include <QProxyStyle>

namespace mmn {

class MqttTabStyle : public QProxyStyle
{
public:
    virtual QSize sizeFromContents(QStyle::ContentsType type,
                                   const QStyleOption *option,
                                   const QSize &size,
                                   const QWidget *widget) const override;
    virtual void drawControl(QStyle::ControlElement element,
                             const QStyleOption *option,
                             QPainter *painter,
                             const QWidget *widget = nullptr) const override;
};

} // namespace mmn

#endif // MMN_UI_MQTT_TAB_STYLE_HPP