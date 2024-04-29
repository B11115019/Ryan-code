from tkinter import *
import time

def update():
    time_string = time.strftime("%I : %M : %S %p")
    time_label.config(text=time_string)

    day_string = time.strftime("%A")
    day_label.config(text=day_string)

    date_string = time.strftime("%Y/%B/%d")
    date_label.config(text=date_string)

    # after(延遲的時間，以毫秒為單位, 要執行的函式, 如果有需要的話，可以傳遞給函式的額外參數。)
    window.after(1000, update)


window = Tk()

time_label = Label(window, font=("Arial", 50), fg="#00FF00", bg="black")
time_label.pack()

day_label = Label(window, font=("Ink Free", 25))
day_label.pack(side=LEFT)

date_label = Label(window, font=("Ink Free", 25))
date_label.pack(side=RIGHT)

update()
window.mainloop()
