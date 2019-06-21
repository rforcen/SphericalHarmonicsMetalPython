import sys

from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import (QApplication, QMainWindow)
from rendererGL import RendererGL
from SphericalHarmonics import SphericalHarmonics, read_code, n_codes, get_code
import random


class sw_widget(RendererGL):
    coords = None
    colors = None
    normals = None
    texture = None
    mesh = None
    win = None
    need_compile = True
    gl_compiled_list = 1

    def __init__(self, mesh, win):
        super(sw_widget, self).__init__()
        self.mesh = mesh
        self.win = win
        self.get_mesh()

        self.setFocusPolicy(Qt.StrongFocus)  # accepts key events

    def get_mesh(self):
        self.coords = self.mesh[0]
        self.normals = self.mesh[1]
        self.colors = self.mesh[2]
        self.texture = self.mesh[3]

    def init(self, gl):
        self.sceneInit(gl)
        gl.glCullFace(gl.GL_FRONT)

    def draw(self, gl):
        def draw_solid(gl):
            if self.coords is not None:
                gl.glEnable(gl.GL_NORMALIZE)

                for ic in range(0, len(self.coords), 4): # tranverse quads
                    gl.glBegin(gl.GL_QUADS)

                    for i in range(ic, ic + 4): # draw the quad
                        gl.glVertex3fv(self.coords[i])
                        gl.glNormal3fv(self.normals[i])
                        gl.glColor3fv(self.colors[i])
                        gl.glTexCoord2fv(self.texture[i])

                    gl.glEnd()

        def compile(gl):
            if self.need_compile:
                gl.glNewList(self.gl_compiled_list, gl.GL_COMPILE)
                draw_solid(gl)
                gl.glEndList()
                self.need_compile = False

        def draw_list(gl):
            compile(gl)
            gl.glCallList(self.gl_compiled_list)

        sc = 0.1
        gl.glScalef(sc, sc, sc)

        draw_list(gl)


class Main(QMainWindow):
    def __init__(self, sh, *args):
        super(Main, self).__init__(*args)

        self.setWindowTitle('Spherical Harmonics, code: ' + get_code())
        self.setCentralWidget(sw_widget(sh, self))
        self.show()


if __name__ == '__main__':
    random.seed()

    resolution, colorMap, code = 256, 9, random.randint(0, n_codes()) # build SH mesh
    mesh = SphericalHarmonics(resolution, colorMap, code)  # res, colorMap index, code

    app = QApplication(sys.argv)
    Main(mesh)

    app.exec_()
