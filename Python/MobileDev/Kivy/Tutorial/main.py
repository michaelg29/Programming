from kivy.app import App
from kivy.uix.widget import Widget
from kivy.uix.label import Label
from kivy.uix.gridlayout import GridLayout
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.textinput import TextInput
from kivy.uix.button import Button
from kivy.uix.widget import Widget
from kivy.properties import ObjectProperty
from kivy.graphics import Rectangle
from kivy.graphics import Line
from kivy.graphics import Color

class Draw(Widget):
    def __init__(self, **kwargs):
        super(Draw, self).__init__(**kwargs)
        with self.canvas:
            Color(1, 0, 0, .5, mode="rgba")
            self.rect = Rectangle(pos=(0, 0), size=(50, 50))
            #Color(1, 1, 0, .5, mode="rgba")
            #self.rect = Rectangle(pos=(300, 300), size=(100, 50))

            Line(points=[20, 30, 400, 500, 60, 500], width=10)

    def on_touch_down(self, touch):
        self.rect.pos = touch.pos

    def on_touch_move(self, touch):
        self.rect.pos = touch.pos

class Touch(Widget):
    btn = ObjectProperty(None)

    def on_touch_down(self, touch): # coordinate of press
        print("Mouse down", touch)
        self.btn.opacity = 0.5

    def on_touch_move(self, touch): # drag
        print("Mouse move", touch)

    def on_touch_up(self, touch): # coordinate of release
        print("Mouse up", touch)
        self.btn.opacity = 1

class MyGrid(Widget):
    fname = ObjectProperty(None)
    lname = ObjectProperty(None)
    email = ObjectProperty(None)

    def btn(self):
        print("Name:", self.fname.text, self.lname.text, "Email:", self.email.text)
        self.fname.text = ""
        self.lname.text = ""
        self.email.text = ""

class MyApp(App):
    def build(self):
        return Draw()

if __name__ == "__main__":
    MyApp().run()