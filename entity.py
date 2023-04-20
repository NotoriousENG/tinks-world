import pygame
from pygame.math import Vector2


class Entity:
    def __init__(self, pos, rot, image, speed):
        self.position = pos
        self.rotation = rot
        self.carImg = image
        self.speed = speed

    def draw(self, surface):
        # rotate the image by 90 degrees
        surface.blit(pygame.transform.rotate(self.carImg, self.rotation), self.position)