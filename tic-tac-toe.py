from tkinter import *
import random


def next_turn(row, col):
    global player

    # buttons[row][col]["text"]拿到這個位子的button字串
    if buttons[row][col]['text'] == "" and check_winner() is False:
        if player == players[0]:
            buttons[row][col]["text"] = player

            if check_winner() is False:
                player = players[1]
                label.config(text=player + " turn")
            elif check_winner() is True:
                label.config(text=player + " wins")
            elif check_winner() == "Tie":
                label.config(text="Tie!")
        else:
            buttons[row][col]["text"] = player

            if check_winner() is False:
                player = players[0]
                label.config(text=player + " turn")
            elif check_winner() is True:
                label.config(text=player + " wins")
            elif check_winner() == "Tie":
                label.config(text="Tie!")


def check_winner():

    for row in range(3):
        if buttons[row][0]['text'] == buttons[row][1]['text'] == buttons[row][2]['text'] != "":
            buttons[row][0].config(bg="green")
            buttons[row][1].config(bg="green")
            buttons[row][2].config(bg="green")
            return True

    for col in range(3):
        if buttons[0][col]['text'] == buttons[1][col]['text'] == buttons[2][col]['text'] != "":
            buttons[0][col].config(bg="green")
            buttons[1][col].config(bg="green")
            buttons[2][col].config(bg="green")
            return True

    if buttons[0][0]['text'] == buttons[1][1]['text'] == buttons[2][2]['text'] != "":
        buttons[0][0].config(bg="green")
        buttons[1][1].config(bg="green")
        buttons[2][2].config(bg="green")
        return True
    elif buttons[0][2]['text'] == buttons[1][1]['text'] == buttons[2][0]['text'] != "":
        buttons[0][2].config(bg="green")
        buttons[1][1].config(bg="green")
        buttons[2][0].config(bg="green")
        return True
    elif empty_spaces() is False:
        return "Tie"
    else:
        return False


def empty_spaces():

    for row in range(3):
        for col in range(3):
            if buttons[row][col]['text'] == "":
                return True

    return False


def new_game():
    global player
    player = random.choice(players)
    label.config(text=player + " turn")

    for row in range(3):
        for col in range(3):
            buttons[row][col].config(text='', bg="#F0F0F0")


window = Tk()
window.title("Tic-Tac-Toe")
players = ["x", "o"]
player = random.choice(players)

buttons = [[0, 0, 0],
           [0, 0, 0],
           [0, 0, 0]]

label = Label(window, text=player + " turn", font=("consolas", 40))
label.pack(side="top")

reset_button = Button(window, text="restart", font=("consolas", 20), command=new_game)
reset_button.pack(side="top")

frame = Frame(window)
frame.pack()

for row in range(3):
    for col in range(3):
        buttons[row][col] = Button(frame, text="", font=("consolas", 40),
                                   width=5, height=2, command=lambda r=row, c=col: next_turn(r, c))
        buttons[row][col].grid(row=row, column=col)

window.mainloop()