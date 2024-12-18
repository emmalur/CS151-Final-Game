#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <functional>

using namespace ftxui;

/**
 * @param play a function pointer to change a calling function's variable
 */
/*
Component mainMenu(std::function<void()> play)
{
    auto button = Button("Play", play, ButtonOption::Animated());

    int where = 0;
    auto container = Container::Vertical({
        Container::Horizontal({
            button,
        }, &where),
    });

    return Renderer(container, [&] {
        return vbox({
            text("The Dungeon of Doom") | bold | center,
            container->Render(),
        });
    });
}*/

Element mainMenuDecorator(Element element) {
  return vbox({
      text("Dungeon of Doom") | bold | center,
      element,
  });
}

Component mainMenu(std::function<void()> play)
{
    class Impl : public ComponentBase
    {
    public:
        Impl(std::function<void()> play)
        {
            
            auto button = Button("Play", play, ButtonOption::Animated());

            auto component = Container::Vertical({
                button,                                
            });

            component |= mainMenuDecorator;    
            
            Add(component);
        }
    };

    return Make<Impl>(std::move(play));
}
