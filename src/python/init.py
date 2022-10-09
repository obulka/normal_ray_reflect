import os


_parent_dir = os.path.dirname(os.path.dirname(__file__))

nuke.pluginAddPath(os.path.join(_parent_dir, "gizmos"))
