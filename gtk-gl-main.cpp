// This came from the "Drawing" example of https://developer.gnome.org/gtk3/3.4/gtk-getting-started.html

#include <string.h>
#include <gtk/gtk.h>
#include "gles-renderer.h"

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

/* Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static gboolean
realize_cb (GtkGLArea *area)
{
    gtk_gl_area_make_current (area);
    if (gtk_gl_area_get_error (area) != NULL) {
        printf("gtk_gl_area_make_current error\n");
        return FALSE;
    }
    printf("init called\n");
    GLES_RENDERER::initGlRenderer();
    return TRUE;
}

/* Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static gboolean
draw_cb (GtkGLArea *area, GdkGLContext *context)
{
    gtk_widget_queue_draw(GTK_WIDGET(area));
    GLES_RENDERER::renderFrame();
    return FALSE;
}

static gboolean
configure_event_cb (GtkWidget         *widget,
                    GdkEventConfigure *event,
                    gpointer           data)
{
  return TRUE;
}

static void
close_window (void)
{
//  if (surface)
//    cairo_surface_destroy (surface);

  gtk_main_quit ();
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *drawing_frame;
  GtkWidget *drawing_area;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Drawing Area");
  gtk_widget_add_events(GTK_WIDGET(window), GDK_SCROLL_MASK);
  g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);
  gtk_container_set_border_width (GTK_CONTAINER (window), 8);

  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add(GTK_CONTAINER (window), vbox);

  drawing_frame = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (drawing_frame), GTK_SHADOW_IN);
  gtk_container_add (GTK_CONTAINER (vbox), drawing_frame);

  drawing_area = gtk_gl_area_new ();
  // TODO kell hozza a gtk3.22
  // https://launchpad.net/~jonathonf/+archive/ubuntu/gtk3.22/+packages
//  gtk_gl_area_set_use_es(GTK_GL_AREA(drawing_area), TRUE);

  /* set a minimum size */
  gtk_widget_set_size_request (drawing_area, 480, 234);

  gtk_container_add (GTK_CONTAINER (drawing_frame), drawing_area);

  /* Signals used to handle the backing surface */
//  g_signal_connect (drawing_area, "draw",
//                    G_CALLBACK (draw_cb), NULL);
  g_signal_connect (drawing_area, "render",
                    G_CALLBACK (draw_cb), NULL);
  g_signal_connect (drawing_area, "realize",
                    G_CALLBACK (realize_cb), NULL);
  g_signal_connect (drawing_area,"configure-event",
                    G_CALLBACK (configure_event_cb), NULL);

  /* Ask to receive events the drawing area doesn't normally
   * subscribe to. In particular, we need to ask for the
   * button press and motion notify events that want to handle.
   */
//  gtk_widget_set_events (drawing_area, gtk_widget_get_events (drawing_area)
//                                     | GDK_BUTTON_PRESS_MASK
//                                     | GDK_POINTER_MOTION_MASK);

  gtk_widget_show_all (window);
}

int main(int argc, char *argv[]) {
  GtkApplication *app;
  int status;

  app = gtk_application_new ("com.testapp", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
