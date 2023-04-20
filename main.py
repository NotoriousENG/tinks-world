import asyncio
import pygame
import colorsys
import sys

from scene import Scene

def hsv_shift(r,g,b, delta):
    h,s,v = colorsys.rgb_to_hsv(r,g,b)
    h = (h + 0.05 * delta) % 1.0
    r,g,b = colorsys.hsv_to_rgb(h,s,v)
    return (r,g,b)

async def main():
    pygame.init()
    pygame.mixer.init()

    display_width = 800
    display_height = 600

    gameDisplay = pygame.display.set_mode([display_width,display_height])
    pygame.display.set_caption("Tink's World")

    current_scene = Scene(display_width, display_height)
    current_scene.begin()

    clear_color = (125, 255, 134)

    clock = pygame.time.Clock()
    crashed = False

    pygame.mixer.music.load('cat_song.ogg', 'ogg')
    pygame.mixer.music.play(-1)
    pygame.mixer.music.set_volume(0.2)

    # Game Loop
    while not crashed:
        delta = float(clock.get_time()) / 1000.0

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                crashed = True

        clear_color = hsv_shift(clear_color[0], clear_color[1], clear_color[2], delta)    

        gameDisplay.fill(clear_color) # fill screen with white (clears the screen)

        current_scene.logic(delta) # update game logic and data
        current_scene.draw(gameDisplay) # draw the next frame
            
        pygame.display.update() # update the screen (all of the drawing commands like blit are rendered to the screen)
        clock.tick(60)
        await asyncio.sleep(0)

    pygame.quit()

if __name__ == "__main__":
    asyncio.run(main())