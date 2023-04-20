import random
import pygame
from entity import Entity
from pygame.math import Vector2

class Scene:

    def __init__(self, width, height):
        self.collectables = []
        self.width = width
        self.height = height
        self.player = None
        self.enemy = None
        self.score = {
            'player': 0,
            'enemy': 0
        }
        self.level = 0
    
    def begin(self):
        self.font = pygame.font.Font('Cattie-Regular.ttf', 64)
        # set scores 
        self.score = {
            'player': 0,
            'enemy': 0
        }
        level_multiplier = 10
        
        # spawn player
        player_pos = Vector2(self.width * 0.45, self.height * 0.8)
        player_sprite = pygame.image.load('tink.png')
        # scale the image to 2x of its original size
        player_sprite = pygame.transform.scale(player_sprite, (player_sprite.get_width() * 2, player_sprite.get_height() * 2))
        player_speed = 300 + level_multiplier * self.level
        self.player = Entity(player_pos, 0, player_sprite, player_speed)

        # spawn enemy other side of the screen
        enemy_pos = Vector2(self.width * 0.45, self.height * 0.2)
        enemy_sprite = pygame.image.load('anya.png')
        # enemy speed scales with level
        enemy_speed = 150 + level_multiplier * self.level 
        self.enemy = Entity(enemy_pos, 180, enemy_sprite, enemy_speed)


        # spawn 10 collectables at random positions ensure they are at least 50 pixels away from each other
        added_positions = []
        for i in range(10):
            pos = Vector2(random.randint(0, self.width - 100), random.randint(0, self.height - 100))
            for p in added_positions:
                # ensure pos has the distance_to attribute
                while Vector2(pos).distance_to(p) < 50:
                    pos = (random.randint(0, self.width - 100), random.randint(0, self.height - 100))
            added_positions.append(pos)
            collectable_sprite = pygame.image.load('wf.png')
            self.collectables.append(Entity(pos, 0, collectable_sprite, 0))
    
    def draw(self, surface):
        for entity in self.collectables:
            entity.draw(surface)
        self.enemy.draw(surface)
        self.player.draw(surface)

        # draw the score
        text = self.font.render('Tink: ' + str(self.score['player']), False, (0, 0, 0))
        surface.blit(text, (10, 10))
        text = self.font.render('Anya: ' + str(self.score['enemy']), False, (0, 0, 0))
        surface.blit(text, (10, 72))

        # draw the level
        text = self.font.render('Level: ' + str(self.level + 1), False, (0, 0, 0))
        surface.blit(text, (540, 10))
    
    def logic(self, delta):
        self.player_logic(delta)
        self.collectable_logic()
        self.enemy_logic(delta)
    
    def collectable_logic(self):
        # if there are no more collectables handle the end of the game
        if len(self.collectables) == 0:
            if self.score['player'] > self.score['enemy']:
                print('Tink wins!')
                self.level += 1
            elif self.score['player'] < self.score['enemy']:
                print('Anya wins!')
            else:
                print('Draw!')
            return self.begin()
        # if the player collides with a collectable, remove it from the list
        for collectable in self.collectables:
            if self.player.position.distance_to(collectable.position) < 50 * 2:
                self.collectables.remove(collectable)
                self.score['player'] += 1
                continue
            if self.enemy.position.distance_to(collectable.position) < 50:
                self.collectables.remove(collectable)
                self.score['enemy'] += 1
            

    def player_logic(self, delta):
        ''' Change x and y based on delta time and input.'''
        pressed = pygame.key.get_pressed()

        input_dir = Vector2(0,0)
        # use WASD or arrow keys to move
        input_dir.x = (pressed[pygame.K_RIGHT] or pressed[pygame.K_d]) - (pressed[pygame.K_LEFT] or pressed[pygame.K_a])
        input_dir.y = (pressed[pygame.K_DOWN] or pressed[pygame.K_s]) - (pressed[pygame.K_UP] or pressed[pygame.K_w])

        if input_dir.length() != 0:
            input_dir = input_dir.normalize()

            # get the angle of the input direction limit to 0, 90, 180, 270
            rot = input_dir.angle_to(Vector2(0,-1))
            self.player.rotation = round(rot / 90) * 90


        self.player.position += input_dir * self.player.speed * delta

        # if the player is off screen by 100 pixels, warp them to the other side
        if self.player.position.x > self.width + 100:
            self.player.position.x = -100
        if self.player.position.x < -100:
            self.player.position.x = self.width + 100
        if self.player.position.y > self.height + 100:
            self.player.position.y = -100
        if self.player.position.y < -100:
            self.player.position.y = self.height + 100

    def enemy_logic(self, delta):
        ''' Enemy moves towards the closest collectable.'''

        enemy_pos = Vector2(self.enemy.position)
        closest = None
        for collectable in self.collectables:
            if closest is None or enemy_pos.distance_to(collectable.position) < enemy_pos.distance_to(closest.position):
                closest = collectable
        
        if closest is not None:
            input_dir = Vector2(closest.position - enemy_pos)
            if input_dir.length() != 0:
                input_dir = input_dir.normalize()

                # get the angle of the input direction limit to 0, 90, 180, 270
                rot = input_dir.angle_to(Vector2(0,-1))
                self.enemy.rotation = round(rot / 90) * 90

            self.enemy.position += input_dir * self.enemy.speed * delta