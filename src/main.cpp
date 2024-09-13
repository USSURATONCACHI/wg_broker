#include <iostream>

#include <toml++/toml.hpp>
#include <wg_broker/whole_app.hpp>

int main() {
    ussur::wg::WholeApp app = ussur::wg::create_app();

    g_main_loop_run(app.loop.get());

    return 0;
}
