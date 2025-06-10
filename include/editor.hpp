#pragma once

#include <gtkmm.h>
#include <gtksourceviewmm.h>
#include <string>
#include <vector>
#include <memory>

class Editor {
public:
    Editor();
    ~Editor();

    void run();

private:
    // Main window and layout
    Gtk::Window main_window;
    Gtk::Box main_box;
    Gtk::Paned h_paned;  // Horizontal paned for sidebar
    Gtk::Box sidebar_box;
    Gtk::Box editor_box;
    Gtk::Notebook editor_notebook;
    Gtk::Statusbar status_bar;

    // Sidebar components
    Gtk::TreeView file_tree;
    Glib::RefPtr<Gtk::TreeStore> file_store;
    Gtk::ScrolledWindow sidebar_scroll;
    Gtk::Button new_file_button;
    Gtk::Button open_folder_button;

    // Menu and toolbar
    Gtk::MenuBar menu_bar;
    Gtk::Toolbar toolbar;

    // File operations
    void on_file_new();
    void on_file_open();
    void on_file_save();
    void on_file_save_as();
    void on_folder_open();
    void on_quit();

    // Editor operations
    void setup_window();
    void setup_menu();
    void setup_toolbar();
    void setup_sidebar();
    void setup_editor_area();
    void setup_status_bar();

    // File tree operations
    void populate_file_tree(const std::string& path);
    void on_file_tree_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
    void on_drag_data_received(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y,
                              const Gtk::SelectionData& selection_data, guint info, guint time);

    // Helper methods
    Gsv::View* create_editor_view();
    void add_tab(const std::string& filename);
    void update_status_bar(const std::string& message);
    std::string get_file_extension(const std::string& filename);
    Glib::RefPtr<Gsv::Language> get_language_for_file(const std::string& filename);
}; 