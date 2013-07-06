
from Tkinter import *
root = Tk()

cheese_var = IntVar()
parrot_var = IntVar(value=1)
check_menu = Menu(tearoff=0)
check_menu.add_checkbutton(label="Cheese", variable=cheese_var)
check_menu.add_checkbutton(label="Parrot", variable=parrot_var)
count = 0

class Top():
    def __init__(self):
        global count
        count += 1
        self.tl = Toplevel(root)
        Label(self.tl, text="Window " + str(count)).pack()
        self.mb = Menubutton(self.tl, text="Push Me", bg='pink')
        self.menu = Menu(self.mb, tearoff=0)
        self.menu.add_cascade(label="Choices", menu=check_menu)
        self.menu.add_command(label="New Window", command=new_top)
        self.mb.config(menu=self.menu)
        self.mb.pack()

def new_top():
    Top()

Top()
root.mainloop()
