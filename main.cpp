#include <gtk/gtk.h>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <exception>

/* Definicja pliku z zawartoœci¹ GUI */
#define UI_FILE "main.ui"
#define UI_SFILE "save.ui"
#define UI_IFILE "info.ui"
#define UI_EFILE "error.ui"



GtkWindow *mainWindow;
GtkWidget *button1;
GtkComboBoxText *dysk;
GtkWidget *scrolled;
GtkWidget *info;
GtkWidget *errWin;
GtkWidget *filname;
GtkWidget *saveWindow;


gchar* directory;
std::string res;
int index;

GtkCheckButton *bF;
GtkCheckButton *bV;
GtkCheckButton *bR;
GtkCheckButton *bX;
GtkCheckButton *bI;
GtkCheckButton *bC;
GtkCheckButton *bB;
GtkCheckButton *bscan;
GtkCheckButton *bforceofflinefix;
GtkCheckButton *bperf;
GtkCheckButton *bspotfix;
GtkCheckButton *bsdcleanup;
GtkCheckButton *bofflinescanandfix;
GtkCheckButton *bfreeorphanedchainds;
GtkCheckButton *bmarkclean;

int selectedInCombo;
bool activeVisable;
int selectedInRadio;

struct SystemFileException : public std::exception
{
	const char * what () const throw ()
    {
    	return "Nieprawidłowe parametry dla wybrajej struktury plików";
    }
};
struct EmptyNameException : public std::exception
{
	const char * what () const throw ()
    {
    	return "Wprowadź nazwę pliku";
    }
};

void ShowError(std::string message);
bool CreateCommond();

GtkWidget * utworz_okno( void )
{
    GtkWidget * okno;
    GtkBuilder * builder;
    GError * error = NULL;

    /* Tworzy obiekt buildera */
    builder = gtk_builder_new();
    /* Wczytuje zawartoœæ interfejsu i sprawdza ewentualne b³êdy */
    if( !gtk_builder_add_from_file( builder, UI_FILE, & error ) )
    {
        g_warning( "Nie mo¿na wczytaæ plilu buildera: %s", error->message );
        g_error_free( error );
    }

    /* £¹czy sygna³y zawarte w pliku interfejsu z odpowiednimi funkcjami */
    gtk_builder_connect_signals( builder, NULL );

    /* Pobiera obiekt z nazw¹ "window1" */
    okno = GTK_WIDGET( gtk_builder_get_object( builder, "window1" ) );

    button1 = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
    dysk = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "comboboxtext1"));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dysk),2);
    scrolled = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow1"));
    gtk_widget_set_visible (scrolled, false);
    activeVisable = false;
    selectedInRadio = 0;
    mainWindow = GTK_WINDOW( gtk_builder_get_object( builder, "window1" ) );
    gtk_window_resize(mainWindow,400,200);

    bF = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "checkbutton1"));
    bV = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "checkbutton2"));
    bR = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "checkbutton3"));
    bX = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "checkbutton4"));
    bI = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "checkbutton5"));
    bC = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "checkbutton6"));
    bB = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "checkbutton7"));
    bscan = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "checkbutton8"));
    bforceofflinefix = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "checkbutton9"));
    bperf = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "checkbutton10"));
    bspotfix = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "checkbutton11"));
    bsdcleanup = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "checkbutton12"));
    bofflinescanandfix = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "checkbutton13"));
    bfreeorphanedchainds = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "checkbutton14"));
    bmarkclean = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "checkbutton15"));

    /* Obiekt buildera nie bêdzie ju¿ nam potrzebny, wiêc go "zwalniamy" */
    g_object_unref( builder );

    return okno;
}

extern "C" G_MODULE_EXPORT void on_comboboxtext1_changed(GtkComboBoxText *b)
{
    selectedInCombo = gtk_combo_box_get_active(GTK_COMBO_BOX(dysk));
}

extern "C" G_MODULE_EXPORT void on_imagemenuitem1_button_press_event(GtkImageMenuItem *b)
{
    gtk_widget_set_visible(scrolled, false);
    activeVisable = false;
    gtk_window_resize(mainWindow,400,200);
}

extern "C" G_MODULE_EXPORT void on_imagemenuitem2_button_press_event(GtkImageMenuItem *b)
{
    gtk_widget_set_visible (scrolled, true);
    activeVisable = true;
}

extern "C" G_MODULE_EXPORT void on_imagemenuitem5_button_press_event(GtkImageMenuItem *b)
{
    exit(3);
}

extern "C" G_MODULE_EXPORT void on_radiobutton1_toggled(GtkRadioButton *b)
{
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b)))
        selectedInRadio = 0;
}

extern "C" G_MODULE_EXPORT void on_radiobutton2_toggled(GtkRadioButton *b)
{
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b)))
        selectedInRadio = 1;
}

extern "C" G_MODULE_EXPORT void on_radiobutton3_toggled(GtkRadioButton *b)
{
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b)))
        selectedInRadio = 2;
}

extern "C" G_MODULE_EXPORT void on_button1_clicked(GtkButton *b)
{
    bool a = CreateCommond();
    if(!a)
        return;
    GtkWidget * okno;
    GtkBuilder * builder;
    GError * error = NULL;

    /* Tworzy obiekt buildera */
    builder = gtk_builder_new();
    /* Wczytuje zawartoœæ interfejsu i sprawdza ewentualne b³êdy */
    if( !gtk_builder_add_from_file( builder, UI_SFILE, & error ) )
    {
        g_warning( "Nie mo¿na wczytaæ plilu buildera: %s", error->message );
        g_error_free( error );
    }

    /* £¹czy sygna³y zawarte w pliku interfejsu z odpowiednimi funkcjami */
    gtk_builder_connect_signals( builder, NULL );

    /* Pobiera obiekt z nazw¹ "window1" */
    okno = GTK_WIDGET( gtk_builder_get_object( builder, "filechooserdialog1" ) );
    filname = GTK_WIDGET(gtk_builder_get_object(builder, "namefile"));
    saveWindow = okno;
    /* Obiekt buildera nie bêdzie ju¿ nam potrzebny, wiêc go "zwalniamy" */
    g_object_unref( builder );



    gtk_widget_show(okno);
}

extern "C" G_MODULE_EXPORT void  on_imagemenuitem10_button_press_event(GtkImageMenuItem *b)
{
    GtkWidget * okno;
    GtkBuilder * builder;
    GError * error = NULL;

    GtkWidget * dialog;

    /* Tworzy obiekt buildera */
    builder = gtk_builder_new();
    /* Wczytuje zawartoœæ interfejsu i sprawdza ewentualne b³êdy */
    if( !gtk_builder_add_from_file( builder, UI_IFILE, & error ) )
    {
        g_warning( "Nie mo¿na wczytaæ plilu buildera: %s", error->message );
        g_error_free( error );
    }

    /* £¹czy sygna³y zawarte w pliku interfejsu z odpowiednimi funkcjami */
    gtk_builder_connect_signals( builder, NULL );

    /* Pobiera obiekt z nazw¹ "window1" */
    okno = GTK_WIDGET( gtk_builder_get_object( builder, "aboutdialog1" ) );
    info = okno;
    dialog = GTK_WIDGET( gtk_builder_get_object( builder, "dialog-action_area1" ) );
    gtk_widget_set_visible (dialog, false);


    //info = GTK_WINDOW(gtk_builder_get_object( builder, "aboutdialog1" ));
    /* Obiekt buildera nie bêdzie ju¿ nam potrzebny, wiêc go "zwalniamy" */
    g_object_unref( builder );

    gtk_widget_show(okno);
}

extern "C" G_MODULE_EXPORT void on_buttonI_clicked(GtkButton *b)
{
    gtk_widget_set_visible (info, false);
}

extern "C" G_MODULE_EXPORT void on_buttone_clicked(GtkButton *b)
{
    gtk_widget_set_visible (errWin, false);
}

extern "C" G_MODULE_EXPORT void on_filechooserdialog1_current_folder_changed(GtkFileChooser *b)
{
    directory = gtk_file_chooser_get_current_folder(b);
}

extern "C" G_MODULE_EXPORT void on_buttonc_clicked(GtkButton *b)
{
    bool udanyZapis = false;

    try{
        const gchar *  name = gtk_entry_get_text(GTK_ENTRY(filname));
        const gchar * dir = directory;

        if(0 == gtk_entry_get_text_length (GTK_ENTRY(filname)))
            throw EmptyNameException();

        std::string fullpath = "";

        int Size=0;

        if(directory != NULL){
            while (dir[Size] != '\0')
            {
                fullpath += dir[Size];
                Size++;
            }
            fullpath += '\\';
        }

        Size = 0;
        while (name[Size] != '\0')
        {
            fullpath += name[Size];
            Size++;
        }

        fullpath += ".bat";


        int n = fullpath.length();
        char char_array[n + 1];

        strcpy(char_array, fullpath.c_str());


        std::ofstream myfile(char_array);
        myfile << res+"\n";
        myfile << "pause\n";
        myfile.close();
        udanyZapis = true;

    }
    catch (EmptyNameException& e)
    {
    	ShowError("Wpisz nazwę pliku!");
    }
    catch (std::exception& e)
    {
    	ShowError("Podczas zapisu wystąpiły problemy");
    }
    if(udanyZapis == true)
        gtk_widget_set_visible (saveWindow,  false);
}

bool CreateCommond()
{

    try{
            res = "chkdsk ";

        int ds = gtk_combo_box_get_active(GTK_COMBO_BOX(dysk));

        switch (ds)
        {
        case 0:
            res += "A:";
            break;
        case 1:
            res += "B:";
            break;
        case 2:
            res += "C:";
            break;
        case 3:
            res += "D:";
            break;
        case 4:
            res += "E:";
            break;
        case 5:
            res += "F:";
            break;
        case 6:
            res += "G:";
            break;
        case 7:
            res += "H:";
            break;
        case 8:
            res += "I:";
            break;
        case 9:
            res += "J:";
            break;
        case 10:
            res += "K:";
            break;
        case 11:
            res += "L:";
            break;
        case 12:
            res += "M:";
            break;
        case 13:
            res += "N:";
            break;
        case 14:
            res += "O:";
            break;
        case 15:
            res += "P:";
            break;
        case 16:
            res += "Q:";
            break;
        case 17:
            res += "R:";
            break;
        case 18:
            res += "S:";
            break;
        case 19:
            res += "T:";
            break;
        case 20:
            res += "U:";
            break;
        case 21:
            res += "V:";
            break;
        case 22:
            res += "W:";
            break;
        case 23:
            res += "X:";
            break;
        case 24:
            res += "Y:";
            break;
        case 25:
            res += "Z:";
            break;
        default:
            res += "C:";
      }

        if(selectedInRadio != 0)
        res += "\\*.*";

        if(gtk_widget_get_visible (scrolled))
        {
            //parametry
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bF)))
            {
                res +=" /F";
            }
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bV)))
            {
                res +=" /V";
            }
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bR)))
            {
                res +=" /R";
            }
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bX)))
            {
                res +=" /X";
            }
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bI)))
            {
                res +=" /I";
                if(selectedInRadio != 0)
                    throw SystemFileException();
            }
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bC)))
            {
                res +=" /C";
                if(selectedInRadio != 0)
                    throw SystemFileException();
            }
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bB)))
            {
                res +=" /B";
                if(selectedInRadio != 0)
                    throw SystemFileException();
            }
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bscan)))
            {
                res +=" /scan";
                if(selectedInRadio != 0)
                    throw SystemFileException();
            }
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bforceofflinefix)))
            {
                res +=" /forceofflinefix";
                if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bscan)))
                    throw SystemFileException();
            }
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bperf)))
            {
                res +=" /perf";
                if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bscan)))
                    throw SystemFileException();
            }
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bspotfix)))
            {
                res +=" /spotfix";
                if(selectedInRadio != 0)
                    throw SystemFileException();
            }
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bsdcleanup)))
            {
                res +=" /sdcleanup";
                if(selectedInRadio != 0)
                    throw SystemFileException();
            }
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bofflinescanandfix)))
            {
                res +=" /offlinescanandfix";
            }
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bfreeorphanedchainds)))
            {
                res +=" /freeorphanedchainds";
                if(selectedInRadio == 0)
                    throw SystemFileException();
            }
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bmarkclean)))
            {
                res +=" /markclean";
                if(selectedInRadio == 0)
                    throw SystemFileException();
            }
        }

    }
    catch (SystemFileException& e)
    {
    	ShowError("Nieprawidłowo dobrane argumęty polecenia!");
    	return false;
    }
    catch (std::exception& e)
    {
    	ShowError("Coś poszło nie tak!");
    	return false;
    }
    return true;
}

void ShowError(std::string message)
{
    int n = message.length();
    char char_array[n + 1];
    strcpy(char_array, message.c_str());


    GtkWidget * okno;
    GtkBuilder * builder;
    GError * error = NULL;

    /* Tworzy obiekt buildera */
    builder = gtk_builder_new();
    /* Wczytuje zawartoœæ interfejsu i sprawdza ewentualne b³êdy */
    if( !gtk_builder_add_from_file( builder, UI_EFILE, & error ) )
    {
        g_warning( "Nie mo¿na wczytaæ plilu buildera: %s", error->message );
        g_error_free( error );
    }

    /* £¹czy sygna³y zawarte w pliku interfejsu z odpowiednimi funkcjami */
    gtk_builder_connect_signals( builder, NULL );

    /* Pobiera obiekt z nazw¹ "window1" */
    okno = GTK_WIDGET( gtk_builder_get_object( builder, "messagedialog1" ) );
    /* Obiekt buildera nie bêdzie ju¿ nam potrzebny, wiêc go "zwalniamy" */
    errWin = okno;
    g_object_unref( builder );

    gtk_message_dialog_set_markup (GTK_MESSAGE_DIALOG(okno),char_array);

    gtk_widget_show(okno);
}

int main( int argc, char * argv[] )
{
    GtkWidget * okno;


//    gtk_set_locale();
    gtk_init( & argc, & argv );

    okno = utworz_okno();
    gtk_widget_show( okno );

    gtk_main();

    return 0;
}
