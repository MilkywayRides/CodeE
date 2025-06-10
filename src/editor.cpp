#include "editor.hpp"
#include <gtkmm/filechooserdialog.h>
#include <fstream>
#include <iostream>

Editor::Editor() : main_box(Gtk::ORIENTATION_VERTICAL), sidebar_box(Gtk::ORIENTATION_VERTICAL), editor_box(Gtk::ORIENTATION_VERTICAL) {
    setup_window();
    setup_menu();
    setup_toolbar();
    setup_sidebar();
    setup_editor_area();
    setup_status_bar();
}

Editor::~Editor() {}

void Editor::run() {
    main_window.show_all();
    auto app = Gtk::Application::create();
    app->run(main_window);
}

void Editor::setup_window() {
    main_window.set_title("VSCode Clone");
    main_window.set_default_size(1200, 800);
    main_window.add(main_box);

    main_box.pack_start(menu_bar, Gtk::PACK_SHRINK);
    main_box.pack_start(toolbar, Gtk::PACK_SHRINK);
    main_box.pack_start(h_paned, Gtk::PACK_EXPAND_WIDGET);
    main_box.pack_start(status_bar, Gtk::PACK_SHRINK);

    // Sidebar (left)
    h_paned.pack1(sidebar_box, false, false);
    // Editor area (right)
    h_paned.pack2(editor_box, true, false);
}

void Editor::setup_menu() {
    // Simple File menu for now
    auto file_menu = Gtk::manage(new Gtk::Menu());
    auto new_item = Gtk::manage(new Gtk::MenuItem("New"));
    new_item->signal_activate().connect(sigc::mem_fun(*this, &Editor::on_file_new));
    file_menu->append(*new_item);
    auto open_item = Gtk::manage(new Gtk::MenuItem("Open"));
    open_item->signal_activate().connect(sigc::mem_fun(*this, &Editor::on_file_open));
    file_menu->append(*open_item);
    auto save_item = Gtk::manage(new Gtk::MenuItem("Save"));
    save_item->signal_activate().connect(sigc::mem_fun(*this, &Editor::on_file_save));
    file_menu->append(*save_item);
    auto save_as_item = Gtk::manage(new Gtk::MenuItem("Save As"));
    save_as_item->signal_activate().connect(sigc::mem_fun(*this, &Editor::on_file_save_as));
    file_menu->append(*save_as_item);
    file_menu->append(*Gtk::manage(new Gtk::SeparatorMenuItem()));
    auto quit_item = Gtk::manage(new Gtk::MenuItem("Quit"));
    quit_item->signal_activate().connect(sigc::mem_fun(*this, &Editor::on_quit));
    file_menu->append(*quit_item);
    auto file_menu_item = Gtk::manage(new Gtk::MenuItem("File"));
    file_menu_item->set_submenu(*file_menu);
    menu_bar.append(*file_menu_item);
}

void Editor::setup_toolbar() {
    auto new_button = Gtk::manage(new Gtk::ToolButton(Gtk::Stock::NEW));
    new_button->signal_clicked().connect(sigc::mem_fun(*this, &Editor::on_file_new));
    toolbar.append(*new_button);
    
    auto open_button = Gtk::manage(new Gtk::ToolButton(Gtk::Stock::OPEN));
    open_button->signal_clicked().connect(sigc::mem_fun(*this, &Editor::on_file_open));
    toolbar.append(*open_button);
    
    auto save_button = Gtk::manage(new Gtk::ToolButton(Gtk::Stock::SAVE));
    save_button->signal_clicked().connect(sigc::mem_fun(*this, &Editor::on_file_save));
    toolbar.append(*save_button);
}

void Editor::setup_editor_area() {
    editor_box.pack_start(editor_notebook, Gtk::PACK_EXPAND_WIDGET);
}

void Editor::setup_status_bar() {
    status_bar.push("Ready");
}

void Editor::on_file_new() {
    add_tab("Untitled");
}

void Editor::on_file_open() {
    try {
        Gtk::FileChooserDialog dialog("Open File", Gtk::FILE_CHOOSER_ACTION_OPEN);
        dialog.set_transient_for(main_window);
        
        // Add buttons
        dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
        dialog.add_button("_Open", Gtk::RESPONSE_OK);
        
        // Add filters
        auto filter_text = Gtk::FileFilter::create();
        filter_text->set_name("Text files");
        filter_text->add_mime_type("text/plain");
        filter_text->add_pattern("*.txt");
        filter_text->add_pattern("*.cpp");
        filter_text->add_pattern("*.hpp");
        filter_text->add_pattern("*.h");
        filter_text->add_pattern("*.c");
        dialog.add_filter(filter_text);
        
        // Add a filter for all files
        auto filter_any = Gtk::FileFilter::create();
        filter_any->set_name("All files");
        filter_any->add_pattern("*");
        dialog.add_filter(filter_any);
        
        // Set the default filter
        dialog.set_filter(filter_text);
        
        // Set the current directory to the user's home directory
        dialog.set_current_folder(Glib::get_home_dir());
        
        int result = dialog.run();
        
        if (result == Gtk::RESPONSE_OK) {
            std::string filename = dialog.get_filename();
            std::cout << "Selected file: " << filename << std::endl;
            
            if (!Glib::file_test(filename, Glib::FILE_TEST_EXISTS)) {
                status_bar.push("Error: File does not exist.");
                return;
            }
            
            if (Glib::file_test(filename, Glib::FILE_TEST_IS_DIR)) {
                status_bar.push("Error: Selected item is a directory. Please select a file.");
                return;
            }
            
            if (!Glib::file_test(filename, Glib::FILE_TEST_IS_REGULAR)) {
                status_bar.push("Error: Selected item is not a regular file.");
                return;
            }
            
            try {
                add_tab(filename);
                status_bar.push("File opened successfully: " + Glib::path_get_basename(filename));
            } catch (const std::exception& e) {
                status_bar.push("Error opening file: " + std::string(e.what()));
            }
        }
    } catch (const std::exception& e) {
        status_bar.push("Error in file dialog: " + std::string(e.what()));
    }
}

void Editor::on_file_save() {
    // TODO: Implement save functionality
    status_bar.push("Save not implemented yet");
}

void Editor::on_file_save_as() {
    // TODO: Implement save as functionality
    status_bar.push("Save As not implemented yet");
}

void Editor::on_quit() {
    main_window.hide();
}

Gsv::View* Editor::create_editor_view() {
    auto view = Gtk::manage(new Gsv::View());
    auto buffer = Gsv::Buffer::create();
    view->set_buffer(buffer);
    view->set_show_line_numbers(true);
    view->set_highlight_current_line(true);
    view->set_auto_indent(true);
    return view;
}

void Editor::add_tab(const std::string& filename) {
    try {
        Gsv::View* view = create_editor_view();
        auto scrolled_window = Gtk::manage(new Gtk::ScrolledWindow());
        scrolled_window->add(*view);
        auto label = Gtk::manage(new Gtk::Label(Glib::path_get_basename(filename)));
        editor_notebook.append_page(*scrolled_window, *label);
        editor_notebook.set_current_page(editor_notebook.get_n_pages() - 1);
        if (filename != "Untitled") {
            std::ifstream file(filename);
            if (!file.is_open()) {
                throw std::runtime_error("Could not open file: " + filename);
            }
            std::string content;
            std::string line;
            while (std::getline(file, line)) {
                content += line + "\n";
            }
            if (file.bad()) {
                throw std::runtime_error("Error reading file: " + filename);
            }
            view->get_buffer()->set_text(content);
            auto language_manager = Gsv::LanguageManager::create();
            std::string extension = Glib::path_get_basename(filename);
            size_t dot_pos = extension.find_last_of('.');
            if (dot_pos != std::string::npos) {
                extension = extension.substr(dot_pos + 1);
                auto language = language_manager->get_language(extension);
                if (language) {
                    auto buffer = Glib::RefPtr<Gsv::Buffer>::cast_dynamic(view->get_buffer());
                    if (buffer) {
                        buffer->set_language(language);
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Error in add_tab: " + std::string(e.what()));
    }
}

void Editor::setup_sidebar() {
    // Placeholder: add a label for now
    auto label = Gtk::manage(new Gtk::Label("Explorer"));
    sidebar_box.pack_start(*label, Gtk::PACK_SHRINK);
} 