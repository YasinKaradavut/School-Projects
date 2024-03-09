import os

MIN_BOARD_SIZE = 5
MAX_BOARD_SIZE = 15
MAX_SHIP_TYPES = 5
MAX_NAME_LENGTH = 50

class Ship:
    def __init__(self, size):
        self.size = size
        self.hits = 0
        self.isSunk = False

class Player:
    def __init__(self, name, ship_count):
        self.name = name
        self.score = 0
        self.ships = [Ship(i + 3) for i in range(ship_count)]

def create_board(size):
    return [['~' for _ in range(size)] for _ in range(size)]

def free_board(board):
    pass  # Python'da garbage collector kullanıldığı için manuel bellek temizleme gerekmez

def initialize_boards(board1, board2):
    for i in range(len(board1)):
        for j in range(len(board1[0])):
            board1[i][j] = '~'
            board2[i][j] = '~'

def print_hidden_board(board):
    size = len(board)
    print("   " + " ".join(str(i + 1) for i in range(size)))
    
    for i in range(size):
        print(chr(ord('A') + i) + " |", end="")
        for j in range(size):
            if board[i][j] == 'X' or board[i][j] == 'O':
                print(board[i][j], end=" ")
            else:
                print("~", end=" ")
        print()

def print_player_info(player, player_name):
    print(f"{player_name}'s Info:")
    print(f"Score: {player.score}")
    for i, ship in enumerate(player.ships):
        print(f"Ship {i + 1} - Size: {ship.size}, Hits: {ship.hits}")

def place_ship(player, board, size, ship_index):
    while True:
        try:
            coords = input(f"Enter the starting coordinates for Ship {ship_index + 1} (size {player.ships[ship_index].size}): ").split()
            row, y = coords[0].upper(), int(coords[1])
            x = ord(row) - ord('A')

            direction = int(input("Downward (1) or Rightward (2)? "))

            if direction == 1:
                if x + player.ships[ship_index].size > size:
                    print("Invalid placement. Try again.")
                else:
                    for i in range(player.ships[ship_index].size):
                        if board[x + i][y - 1] != '~':
                            print("Invalid placement. Try again.")
                            break
                        else:
                            board[x + i][y - 1] = str(ship_index + 1)
                    break
            elif direction == 2:
                if y + player.ships[ship_index].size > size:
                    print("Invalid placement. Try again.")
                else:
                    for i in range(player.ships[ship_index].size):
                        if board[x][y + i - 1] != '~':
                            print("Invalid placement. Try again.")
                            break
                        else:
                            board[x][y + i - 1] = str(ship_index + 1)
                    break
            else:
                print("Invalid direction. Try again.")
        except (ValueError, IndexError):
            print("Invalid input. Try again.")

def place_ships(player, board, size, ship_count):
    for i in range(ship_count):
        player.ships[i].size = i + 3
        player.ships[i].hits = 0
        player.ships[i].isSunk = False

        print_hidden_board(board)
        print_player_info(player, "Player")
        place_ship(player, board, size, i)
        os.system("clear")  # Ekranı temizle

def make_move(player, opponent_board, size, ship_count):
    while True:
        try:
            coords = input("Enter the coordinates to make a move: ").split()
            row, y = coords[0].upper(), int(coords[1])
            x = ord(row) - ord('A')

            if not (0 <= x < size and 1 <= y <= size):
                print("Invalid coordinates. Try again.")
                continue

            if opponent_board[x][y - 1] == 'X' or opponent_board[x][y - 1] == 'O':
                print("Already guessed that location. Try again.")
                continue

            if opponent_board[x][y - 1] != '~':
                ship_index = int(opponent_board[x][y - 1]) - 1
                print("Hit!")
                opponent_board[x][y - 1] = 'X'
                player.ships[ship_index].hits += 1

                if player.ships[ship_index].hits == player.ships[ship_index].size:
                    print("You sank a ship!")
                    player.ships[ship_index].isSunk = True
                    player.score += player.ships[ship_index].size

                    all_sunk = all(ship.isSunk for ship in player.ships)
                    if all_sunk:
                        print("All opponent's ships are sunk! You win!")
                        return False  # Oyunu bitir
            else:
                print("Miss!")
                opponent_board[x][y - 1] = 'O'

            return True  # Oyun devam ediyor
        except (ValueError, IndexError):
            print("Invalid input. Try again.")

def main():
    board_size = int(input(f"Enter the board size (between {MIN_BOARD_SIZE} and {MAX_BOARD_SIZE}): "))

    if not MIN_BOARD_SIZE <= board_size <= MAX_BOARD_SIZE:
        print("Invalid board size. Exiting...")
        return 1

    ship_count = int(input("Enter the number of ships: "))

    if not 1 <= ship_count <= MAX_SHIP_TYPES:
        print("Invalid number of ships. Exiting...")
        return 1

    player1_board = create_board(board_size)
    player2_board = create_board(board_size)

    player1 = Player(input("Enter Player 1's name: "), ship_count)
    player2 = Player(input("Enter Player 2's name: "), ship_count)

    print(f"{player1.name}, place your ships:")
    place_ships(player1, player1_board, board_size, ship_count)

    print(f"{player2.name}, place your ships:")
    place_ships(player2, player2_board, board_size, ship_count)

    current_player = 1

    while True:
        if current_player == 1:
            os.system("clear")
            print(f"{player1.name}'s Turn:")
            print_hidden_board(player2_board)
            print_player_info(player1, player1.name)
            if not make_move(player1, player2_board, board_size, ship_count):
                break  # Oyun bitti
        else:
            os.system("clear")
            print(f"{player2.name}'s Turn:")
            print_hidden_board(player1_board)
            print_player_info(player2, player2.name)
            if not make_move(player2, player1_board, board_size, ship_count):
                break  # Oyun bitti

        current_player = 3 - current_player  # Sıradaki oyuncu

    print("Final Scores:")
    print_player_info(player1, player1.name)
    print_player_info(player2, player2.name)

    free_board(player1_board)
    free_board(player2_board)

if __name__ == "__main__":
    main()
