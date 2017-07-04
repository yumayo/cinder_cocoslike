#pragma once
#include <cinder/app/App.h>
namespace treelike
{
class app_delegate : public cinder::app::App
{
private:
    void setup( ) final override;
    void cleanup( ) final override;
    void update( ) final override;
    void draw( ) final override;
    void mouseDown( cinder::app::MouseEvent event ) final override;
    void mouseMove( cinder::app::MouseEvent event ) final override;
    void mouseDrag( cinder::app::MouseEvent event ) final override;
    void mouseUp( cinder::app::MouseEvent event ) final override;
    void touchesBegan( cinder::app::TouchEvent event ) final override;
    void touchesMoved( cinder::app::TouchEvent event ) final override;
    void touchesEnded( cinder::app::TouchEvent event ) final override;
    void keyDown( cinder::app::KeyEvent event ) final override;
    void keyUp( cinder::app::KeyEvent event ) final override;
private:
    static int const _INVALID_ID;
    int _touch_id = _INVALID_ID;
    double _prev_second = 0.0;
};
}
