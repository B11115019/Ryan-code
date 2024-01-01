def new_game():

    guesses = []
    correct_guesses = 0
    question_num = 1

    for key in question:
        print("-----------------------------")
        print(key)
        for i in option[question_num - 1]:
            print(i)
        guess = input("Enter  (A, B, C, or D)").upper()
        guesses.append(guess)
        correct_guesses += check_answer(question.get(key), guess)
        question_num += 1

    display_score(correct_guesses, guesses)

def check_answer(answer, guess):
    if answer == guess:
        print("CORRECT!!")
        return 1
    else:
        return 0


def display_score(correct_guesses, guesses):
    print("--------------------")
    print("RESULTS")
    print("--------------------")

    print("Answers: ", end="")
    for i in question:
        print(question.get(i), end=" ")
    print()

    print("Guesses: ", end="")
    for i in guesses:
        print(i, end=" ")
    print()

    score = int(correct_guesses/len(question)*100)
    print("Your score is: " + str(score) + "%")


def play_game():
    response = input("Do you want to play again? (yes or no): ").upper()

    if response == "YES":
        return True
    else:
        return False


question = {
    "Who created Python?: ": "A",
    "What year was Python created?: ": "B",
    "Python is tributed to which comedy group?: ": "C",
    "Is the Earth round?: ": "A"
}

option = [["A. Guido van Rossum", "B. Elon Musk", "C. Bill Gates", "D. Mark Zuckerburg"],
          ["A. 1989", "B. 1991", "C. 2000", "D. 2016"],
          ["A. Lonely Island", "B. Smosh", "C. Monty Python", "D. SNL"],
          ["A. True", "B. False", "C. sometimes", "D. What's Earth"]]

new_game()

while play_game():
    new_game()

print("Bye!")
