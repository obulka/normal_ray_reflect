"""Add the gizmo to the menu"""


_toolbar = nuke.toolbar("Nodes")

_sdf_menu = _toolbar.addMenu("RayReflect")
_sdf_menu.addCommand("N_RayReflect", "nuke.createNode('N_RayReflect')")
