import random
import time
from pynput import keyboard
import os
import webbrowser
import sys

class GameElementType:
    SNAKE_HEAD = 1
    SNAKE_BODY = 2
    FOOD = 3

class Direction:
    UP = (0, -1)
    DOWN = (0, 1)
    LEFT = (-1, 0)
    RIGHT = (1, 0)

key_to_direction = {
    'w': Direction.UP,
    'a': Direction.LEFT,
    's': Direction.DOWN,
    'd': Direction.RIGHT,
}

class GameElement:

    def __init__(self, position : tuple, element_type: GameElementType) -> None:
        self.position = position
        self.element_type = element_type

    def __eq__(self, other: 'GameElement'):
        return self.position == other.position
    
    def move(self, position : tuple):
        self.position = position

    def __str__(self):
        if self.element_type == GameElementType.SNAKE_HEAD:
            return "@@"
        if self.element_type == GameElementType.SNAKE_BODY:
            return "=="
        if self.element_type == GameElementType.FOOD:
            return "##"
        
class Screen:

    def __init__(self, width : int, height : int) -> None:
        self.width = width
        self.height = height
        self.screen = [['  ' for _ in range(width)] for _ in range(height)]

    def update_element(self, element : GameElement):
        self.screen[element.position[1]][element.position[0]] = str(element)
    
    def remove_element(self, element : GameElement):
        self.screen[element.position[1]][element.position[0]] = '  '

    def print_screen(self):
        top_bottom = '+' + '-' * self.width * 2 + '+'
        print(top_bottom)
        for i in range(self.height):
            row = '|'
            for j in range(self.width):
                row += self.screen[i][j]
            row += '|'
            print(row)
        print(top_bottom)
    
def get_new_pos(start_positiont : tuple, direction : Direction):
    return (start_positiont[0] + direction[0], start_positiont[1] + direction[1])

def out_of_bounds(position : tuple, move : Direction, width : int, height : int) -> bool:
    new_pos = get_new_pos(position, move)
    if new_pos[0] < 0 or new_pos[0] >= width:
        return True
    if new_pos[1] < 0 or new_pos[1] >= height:
        return True
    return False

class Snake:

    def __init__(self, start_position : tuple, screen : Screen) -> None:
        self.screen = screen
        self.head =  GameElement(position=start_position, element_type=GameElementType.SNAKE_HEAD)
        self.screen.update_element(self.head)
        self.body = []
        self.lenght = 1

    def move(self, direction : Direction, apple : GameElement):
        new_position = get_new_pos(self.head.position, direction)
        head_position = (self.head.position[0], self.head.position[1])
        # check if the new possition is not a snake body
        for idx, _ in enumerate(self.body):
            element : GameElement = self.body[idx]
            if new_position == element.position:
                return 1 # 1 is for fail, eating snake body

        self.screen.remove_element(self.head)
        self.head.move(new_position) 
        self.screen.update_element(self.head)

        if self.head.position == apple.position:
            # need to create a new element, to fill the empty space
            new_element = GameElement(position=head_position, element_type=GameElementType.SNAKE_BODY)
            self.body.insert(0, new_element)
            self.screen.update_element(new_element)
            self.lenght += 1
            if self.lenght == 4: # happy end :)
                sys.exit(2)

            return 2 # in this case, we dont remove the last tail element
        
        if (not self.body):
            return 0 # we have allready move the head, and it is the last element
        
        # in this case we remove the last tail element, change its position to the last head position, and put it after the head
        last_body_element : GameElement = self.body[-1]
        last_body_element = self.body.pop(-1)
        self.screen.remove_element(last_body_element)
        last_body_element.move(head_position)
        self.body.insert(0, last_body_element)
        self.screen.update_element(last_body_element)
        return 0 # 0 is for success, just move the snake
    
class GAME:

    def __init__(self, width : int, height : int) -> None:
        self.width = width
        self.height = height

        if not self.width%2 or not self.height%2:
            raise ValueError("Width and height must be odd numbers")
        
        mid_x = self.width//2
        mid_y = self.height//2
        self.screen = Screen(width, height)
        
        self.snake = Snake(start_position=(mid_x, mid_y), screen=self.screen)
        self.food = None
        self.direction = None
        self.generate_food(True)

    def set_direction(self, direction : Direction):
        self.direction = direction

    def generate_food(self, test) -> bool:

        if test:
            # remove later
            self.food = GameElement(position=(5, 0), element_type=GameElementType.FOOD)
            self.snake.screen.update_element(self.food)
            return False

        if (self.snake.lenght == self.width * self.height):
            return True

        while 1:
            x = random.randint(0, self.width-1)
            y = random.randint(0, self.height-1)

            # now go throught the snake, and check, if the food is not on the snake

            if (x, y) == self.snake.head.position:
                continue
            
            good = True
            for idx, _ in enumerate(self.snake.body):
                element : GameElement = self.snake.body[idx]
                if (x, y) == element.position:
                    good = False
                    break
            if good:
                break
        
        self.food = GameElement(position=(x, y), element_type=GameElementType.FOOD)
        self.snake.screen.update_element(self.food)
        return False
    
    def update_model(self, direction : Direction) -> int:

        # handle going back to the snake
        if self.direction == Direction.UP and direction == Direction.DOWN:
            direction = self.direction
        if self.direction == Direction.DOWN and direction == Direction.UP:
            direction = self.direction
        if self.direction == Direction.LEFT and direction == Direction.RIGHT:
            direction = self.direction
        if self.direction == Direction.RIGHT and direction == Direction.LEFT:
            direction = self.direction

        self.direction = direction

        return_code = self.snake.move(direction, self.food)
        if return_code == 2:
            return self.generate_food(False) # 0 - moved the snake, 1 - end game, win
        if return_code == 1:
            return 2 # 2 is for fail
        return 0 # 0 is for success
    
    def generate_frame(self, direction : Direction) -> int:

        if not direction:
            direction = self.direction

        snake_out_of_bounds = out_of_bounds(self.snake.head.position, direction, self.width, self.height)
        if snake_out_of_bounds:
            return 2 # 2 is for fail
        
        result = self.update_model(direction) # play the move
        
        # generate the frame
        self.snake.screen.print_screen()
        return result
    
    def play(self, direction : Direction) -> bool:
        os.system('clear')
        result = self.generate_frame(direction)
        if result == 2:
            print("Game over, you failed ...")
            return False
        if result == 1:
            print("You win !!")
            return False
        return True

class SnakeKeyHandler:
    def __init__(self):
        self.current_key = None
        self.listener = keyboard.Listener(on_press=self._on_key_press)
        self.listener.start()

    def _on_key_press(self, key):
        try:
            if key.char in key_to_direction:
                self.current_key = key_to_direction[key.char]
        except AttributeError:
            # Ignore special keys
            pass

    def get_snake_direction_blocking(self):
        """
        Blocks until a valid key is pressed.
        """
        while self.current_key is None:
            time.sleep(0.01)  # Wait for a valid input
        direction = self.current_key
        self.current_key = None  # Reset after returning
        return direction

    def get_snake_direction_timeout(self, timeout):
        """
        Waits for a valid key press until timeout.
        """
        start_time = time.time()
        while time.time() - start_time < timeout:
            if self.current_key:
                direction = self.current_key
                self.current_key = None  # Reset after returning
                return direction
            time.sleep(0.01)  # Avoid busy-waiting
        return None  # Return None if timeout occurs

if __name__ == "__main__":

    try:

        timeout = 0.2
        handler = SnakeKeyHandler()
        game = GAME(width=15, height=15)

        game.screen.print_screen()
        print("== Press w/a/s/d to start the game ==")
        direction = handler.get_snake_direction_blocking()
        game.set_direction(direction)

        while True:
            if not game.play(direction):
                break
            print("== Press w/a/s/d to move ==")
            direction = handler.get_snake_direction_timeout(timeout) or direction

        print("Thanks for playing!")

    except SystemExit as e:
        if e.code == 2:
            # Trigger action for exit code 2
            print("Opening a video because exit code was 2.")
            webbrowser.open("https://www.youtube.com/watch?v=UR7tujNjfo4")
        raise  # Re-raise the exit for other handlers or parent processes
