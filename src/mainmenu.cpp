#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <functional>

// Heavy Insperation from https://github.com/ArthurSonzogni/termBreaker for figuring out fxtui

using namespace ftxui;

Element mainMenuDecorator(Element element) {
  return vbox({
      text("Dungeon of Doom") | bold | center,
      element,
  });
}

Component mainMenu(std::function<void()> play, std::function<void()> quit)
{
    class Impl : public ComponentBase
    {
    public:
        Impl(std::function<void()> play, std::function<void()> quit)
        {
            
            auto playButton = Button("Play", play, ButtonOption::Animated());
            auto exitButton = Button("Exit", quit, ButtonOption::Animated());
            
            // TODO: implement these buttons' functions
            auto saveButton = Button("Save", {play}, ButtonOption::Animated());
            auto loadButton = Button("Load", {play}, ButtonOption::Animated());

            auto component = Container::Vertical({
                playButton,
                saveButton,
                loadButton,
                exitButton,                                
            });

            component |= mainMenuDecorator;    
            
            Add(component);
        }
    };

    return Make<Impl>(std::move(play), std::move(quit));
}
