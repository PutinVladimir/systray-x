/*
 *  Qt includes
 */
#include <QWidget>
#include <QWindow>

/*
 *  Main include
 */
#include "windowctrl.h"

/*
 *  System includes
 */
#include "preferences.h"

/*
 *  Constructor
 */
WindowCtrl::WindowCtrl( Preferences* pref, QObject *parent ) :
#ifdef Q_OS_UNIX
    WindowCtrlUnix( parent )
#elif defined Q_OS_WIN
    WindowCtrlWin( parent )
#else
    public QObject
#endif
{
    /*
     *  Store preferences
     */
    m_pref = pref;

    /*
     *  Initialize
     */
    m_minimize_hide = m_pref->getMinimizeHide();
}


void    WindowCtrl::slotWindowTest1()
{
    emit signalConsole("Test 1 started");

    // Do something.

    findWindow( "- Mozilla Thunderbird" );
    displayWindowElements( "- Mozilla Thunderbird" );
//    findWindow( 4313 );

//    captureWindow( "- Mozilla Thunderbird" );

    emit signalConsole("Test 1 done");
}


void    WindowCtrl::slotWindowTest2()
{
    emit signalConsole("Test 2 started");

    // Do something.

    emit signalConsole("Test 2 done");
}


void    WindowCtrl::slotWindowTest3()
{
    emit signalConsole("Test 3 started");

    // Do something.

    emit signalConsole("Test 3 done");
}


/*
 *  Handle window title signal
 */
void    WindowCtrl::slotWindowTitle( QString title )
{
    /*
     *  Store the window title
     */
    m_window_title = title;

    /*
     *  Get the window IDs
     */
    findWindow( title );
}


/*
 *  Handle change in minimizeHide state
 */
void    WindowCtrl::slotMinimizeHideChange()
{
    m_minimize_hide = m_pref->getMinimizeHide();
}


/*
 *  Handle change in window state
 */
void    WindowCtrl::slotWindowState( QString state )
{
    if( m_state != state )
    {
        m_state = state;

        if( state == "normal" )
        {
            foreach( quint64 win_id, getWinIds() )
            {
                hideWindow( win_id, false );
            }
        }
        else
        {
            foreach( quint64 win_id, getWinIds() )
            {
                hideWindow( win_id, m_minimize_hide );
            }
        }

        emit signalConsole( "New state: " + state );
    }
}


/*
 *  Handle show / hide signal
 */
void    WindowCtrl::slotShowHide()
{
    if( m_state == "minimized" )
    {
        m_state = "normal";

        foreach( quint64 win_id, getWinIds() )
        {
            normalizeWindow( win_id );

            emit signalConsole("Normalize");
        }

//        emit signalWindowNormal();    // TB window control

    } else {
        m_state = "minimized";

        foreach( quint64 win_id, getWinIds() )
        {
            minimizeWindow( win_id, m_minimize_hide );

            emit signalConsole("Minimize");
        }

//        emit signalWindowMinimize();  // TB window control

    }
}


/*
 *  Handle close signal
 */
void    WindowCtrl::slotClose()
{
    foreach( quint64 win_id, getWinIds() )
    {
        deleteWindow( win_id );
    }
}
