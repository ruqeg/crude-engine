import bpy


def export_scene(blender_scene, scene_file_path):
    """Exports the given bpy.types.scene to the *.crs file at the given path.
    """
    print()
    print("-###- Beginning Crude renderer export to %s. -###-" % scene_file_path)


class CrudeExportOperator(bpy.types.Operator):
    """This operator exports Blender scenes to the crude renderer (*.crs)
    """

    def invoke(self, context, _event):
        """Shows a file selection dialog for the *.crs file.
        """
        context.window_manager.fileselect_add(self)
        return {"RUNNING_MODAL"}

    def execute(self, context):
        """Perfoms the export using the current settings and context.
        """
        export_scene(context.scene, bpy.path.abspath(self.filepath))
        return {"FINISHED"}
    
    # Settings of this operator
    filepath: bpy.props.StringProperty(name="file path", description="Path to the *.crs file.", subtype="FILE_PATH")

    # Controls file extenssion filters in the dialog
    filter_glob: bpy.props.StringProperty(default="*.crs", options={'HIDDEN'}, maxlen=255)
    filename_ext = ".crs"

    # Some more meta-data
    bl_idname = "export_scene.crude_renderer"
    bl_label = "Export crude renderer scene"
    bl_options = {"PRESET"}


def add_export_operator_to_menu(self, context):
    self.layout.operator(CrudeExportOperator.bl_idname, text="Crude renderer scene (.crs)")


def register():
    bpy.utils.register_class(CrudeExportOperator)
    bpy.types.TOPBAR_MT_file_export.append(add_export_operator_to_menu)


def unregister():
    bpy.utils.unregister_class(CrudeExportOperator)
    bpy.types.TOPBAR_MT_file_export.remove(add_export_operator_to_menu)


if __name__ == "__main__":
    register()
