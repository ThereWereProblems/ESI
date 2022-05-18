#include <gtk/gtk.h>
#include <string>
#include <cmath>
#include <stdlib.h>
/* Definicja pliku z zawartoœci¹ GUI */
#define UI_FILE "main.ui"
#define UI_SFILE "save.ui"

GtkWindow *mainWindow;
GtkWidget *button1;
GtkComboBoxText *dysk;
GtkWidget *scrolled;

gchar* directory;

int selectedInCombo;
bool activeVisable;
int selectedInRadio;


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

    /*
    kwota = GTK_WIDGET(gtk_builder_get_object(builder, "entry1"));
    ileRat = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "spinbutton1"));
    odstep = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "comboboxtext1"));
    oprocentowanie = GTK_WIDGET(gtk_builder_get_object(builder, "entry2"));
    */

    //gtk_combo_box_set_active(GTK_COMBO_BOX(odstep),0);

    //GtkAdjustment  * adj = (GtkAdjustment *) gtk_adjustment_new(1, 1, 24, 1, 1, 0);
    //gtk_spin_button_set_adjustment(ileRat,adj);

    //wynik = GTK_WIDGET(gtk_builder_get_object(builder, "label7"));


    /* Obiekt buildera nie bêdzie ju¿ nam potrzebny, wiêc go "zwalniamy" */
    g_object_unref( builder );

    return okno;
}

void Rata(){
    /*
    const gchar * kw = gtk_entry_get_text(GTK_ENTRY(kwota));
    const gchar * il = gtk_entry_get_text(GTK_ENTRY(ileRat));
    const int od = gtk_combo_box_get_active(GTK_COMBO_BOX(odstep));
    const gchar * op = gtk_entry_get_text(GTK_ENTRY(oprocentowanie));


    double s = (float)strtod(kw,NULL);
    if(s<100 || s> 100000){
        gtk_label_set_text(GTK_LABEL(wynik), "Nieprawidłowa wartość kredytu");
        return;
    }

    double n = (float)strtod(il,NULL);
    double x;
    if(od==0)
        x=1;
    else if(od==1)
        x=2;
    else if(od==2)
        x=3;
    else if(od==3)
        x=6;
    else if(od==4)
        x=12;
    else if(od==5)
        x=24;

    double R = (float)strtod(op,NULL);
    if(R<0 || R> 50){
        gtk_label_set_text(GTK_LABEL(wynik), "Nieprawidłowa wartość oprocentowania");
        return;
    }

    double r = (x*R)/1200;

    double q = 1+r;

    double A = s*pow(q,n)*((q-1)/(pow(q,n)-1));

    char v_str[20];

    sprintf(v_str, "%.2f", A);

    gtk_label_set_text(GTK_LABEL(wynik), v_str);
    */
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
    selectedInRadio = 0;
}

extern "C" G_MODULE_EXPORT void on_radiobutton2_toggled(GtkRadioButton *b)
{
    selectedInRadio = 1;
}

extern "C" G_MODULE_EXPORT void on_radiobutton3_toggled(GtkRadioButton *b)
{
    selectedInRadio = 2;
}

extern "C" G_MODULE_EXPORT void on_button1_clicked(GtkRadioButton *b)
{
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

    /* Obiekt buildera nie bêdzie ju¿ nam potrzebny, wiêc go "zwalniamy" */
    g_object_unref( builder );

    gtk_widget_show(okno);
}

extern "C" G_MODULE_EXPORT void on_filechooserdialog1_current_folder_changed(GtkFileChooser *b)
{
    directory = gtk_file_chooser_get_current_folder(b);
}

extern "C" G_MODULE_EXPORT void on_buttonc_clicked(GtkButton *b)
{
    printf(directory);
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
