/*************************************************************************/
/*  visual_server.h                                                      */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2016 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#ifndef VISUAL_SERVER_H
#define VISUAL_SERVER_H


#include "rid.h"
#include "variant.h"
#include "math_2d.h"
#include "bsp_tree.h"
#include "geometry.h"
#include "object.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class VisualServer : public Object {

	OBJ_TYPE( VisualServer, Object );

	static VisualServer *singleton;

	int mm_policy;

	DVector<String> _shader_get_param_list(RID p_shader) const;
	void _camera_set_orthogonal(RID p_camera,float p_size,float p_z_near,float p_z_far);
	void _canvas_item_add_style_box(RID p_item, const Rect2& p_rect, const Rect2& p_source, RID p_texture,const Vector<float>& p_margins, const Color& p_modulate=Color(1,1,1));
	Array _get_array_from_surface(uint32_t p_format,DVector<uint8_t> p_vertex_data,int p_vertex_len,DVector<uint8_t> p_index_data,int p_index_len) const;

protected:
	RID _make_test_cube();
	void _free_internal_rids();
	RID test_texture;
	RID white_texture;
	RID test_material;
	RID material_2d[16];


	Error _surface_set_data(Array p_arrays,uint32_t p_format,uint32_t *p_offsets,uint32_t p_stride,DVector<uint8_t> &r_vertex_array,int p_vertex_array_len,DVector<uint8_t> &r_index_array,int p_index_array_len,AABB &r_aabb,Vector<AABB> r_bone_aabb);

	static VisualServer* (*create_func)();
	static void _bind_methods();
public:

	static VisualServer *get_singleton();
	static VisualServer *create();



	enum {

		NO_INDEX_ARRAY=-1,
		ARRAY_WEIGHTS_SIZE=4,
		CANVAS_ITEM_Z_MIN=-4096,
		CANVAS_ITEM_Z_MAX=4096,



		MAX_CURSORS = 8,
	};

	/* TEXTURE API */

	enum TextureFlags {
		TEXTURE_FLAG_MIPMAPS=1, /// Enable automatic mipmap generation - when available
		TEXTURE_FLAG_REPEAT=2, /// Repeat texture (Tiling), otherwise Clamping
		TEXTURE_FLAG_FILTER=4, /// Create texure with linear (or available) filter
		TEXTURE_FLAG_ANISOTROPIC_FILTER=8,
		TEXTURE_FLAG_CONVERT_TO_LINEAR=16,
		TEXTURE_FLAG_MIRRORED_REPEAT=32, /// Repeat texture, with alternate sections mirrored
		TEXTURE_FLAG_CUBEMAP=2048,
		TEXTURE_FLAG_USED_FOR_STREAMING=4096,
		TEXTURE_FLAGS_DEFAULT=TEXTURE_FLAG_REPEAT|TEXTURE_FLAG_MIPMAPS|TEXTURE_FLAG_FILTER
	};

	enum CubeMapSide {

		CUBEMAP_LEFT,
		CUBEMAP_RIGHT,
		CUBEMAP_BOTTOM,
		CUBEMAP_TOP,
		CUBEMAP_FRONT,
		CUBEMAP_BACK
	};


	virtual RID texture_create()=0;
	RID texture_create_from_image(const Image& p_image,uint32_t p_flags=TEXTURE_FLAGS_DEFAULT); // helper
	virtual void texture_allocate(RID p_texture,int p_width, int p_height,Image::Format p_format,uint32_t p_flags=TEXTURE_FLAGS_DEFAULT)=0;
	virtual void texture_set_data(RID p_texture,const Image& p_image,CubeMapSide p_cube_side=CUBEMAP_LEFT)=0;
	virtual Image texture_get_data(RID p_texture,CubeMapSide p_cube_side=CUBEMAP_LEFT) const=0;
	virtual void texture_set_flags(RID p_texture,uint32_t p_flags) =0;
	virtual uint32_t texture_get_flags(RID p_texture) const=0;
	virtual Image::Format texture_get_format(RID p_texture) const=0;
	virtual uint32_t texture_get_width(RID p_texture) const=0;
	virtual uint32_t texture_get_height(RID p_texture) const=0;
	virtual void texture_set_size_override(RID p_texture,int p_width, int p_height)=0;

	virtual void texture_set_path(RID p_texture,const String& p_path)=0;
	virtual String texture_get_path(RID p_texture) const=0;

	virtual void texture_set_shrink_all_x2_on_set_data(bool p_enable)=0;

	struct TextureInfo {
		RID texture;
		Size2 size;
		Image::Format format;
		int bytes;
		String path;
	};

	virtual void texture_debug_usage(List<TextureInfo> *r_info)=0;

	/* SKYBOX API */

	virtual RID skybox_create()=0;
	virtual void skybox_set_texture(RID p_skybox,RID p_cube_map,int p_radiance_size)=0;

	/* SHADER API */

	enum ShaderMode {

		SHADER_SPATIAL,
		SHADER_CANVAS_ITEM,
		SHADER_LIGHT,
		SHADER_MAX
	};


	virtual RID shader_create(ShaderMode p_mode=SHADER_SPATIAL)=0;

	virtual void shader_set_mode(RID p_shader,ShaderMode p_mode)=0;
	virtual ShaderMode shader_get_mode(RID p_shader) const=0;

	virtual void shader_set_code(RID p_shader, const String& p_code)=0;
	virtual String shader_get_code(RID p_shader) const=0;
	virtual void shader_get_param_list(RID p_shader, List<PropertyInfo> *p_param_list) const=0;

	virtual void shader_set_default_texture_param(RID p_shader, const StringName& p_name, RID p_texture)=0;
	virtual RID shader_get_default_texture_param(RID p_shader, const StringName& p_name) const=0;


	/* COMMON MATERIAL API */

	virtual RID material_create()=0;

	virtual void material_set_shader(RID p_shader_material, RID p_shader)=0;
	virtual RID material_get_shader(RID p_shader_material) const=0;

	virtual void material_set_param(RID p_material, const StringName& p_param, const Variant& p_value)=0;
	virtual Variant material_get_param(RID p_material, const StringName& p_param) const=0;

	virtual void material_set_line_width(RID p_material, float p_width)=0;

	/* MESH API */

	enum ArrayType {

		ARRAY_VERTEX=0,
		ARRAY_NORMAL=1,
		ARRAY_TANGENT=2,
		ARRAY_COLOR=3,
		ARRAY_TEX_UV=4,
		ARRAY_TEX_UV2=5,
		ARRAY_BONES=6,
		ARRAY_WEIGHTS=7,
		ARRAY_INDEX=8,
		ARRAY_MAX=9
	};

	enum ArrayFormat {
		/* ARRAY FORMAT FLAGS */
		ARRAY_FORMAT_VERTEX=1<<ARRAY_VERTEX, // mandatory
		ARRAY_FORMAT_NORMAL=1<<ARRAY_NORMAL,
		ARRAY_FORMAT_TANGENT=1<<ARRAY_TANGENT,
		ARRAY_FORMAT_COLOR=1<<ARRAY_COLOR,
		ARRAY_FORMAT_TEX_UV=1<<ARRAY_TEX_UV,
		ARRAY_FORMAT_TEX_UV2=1<<ARRAY_TEX_UV2,
		ARRAY_FORMAT_BONES=1<<ARRAY_BONES,
		ARRAY_FORMAT_WEIGHTS=1<<ARRAY_WEIGHTS,
		ARRAY_FORMAT_INDEX=1<<ARRAY_INDEX,

		ARRAY_COMPRESS_BASE=(ARRAY_INDEX+1),
		ARRAY_COMPRESS_VERTEX=1<<(ARRAY_VERTEX+ARRAY_COMPRESS_BASE), // mandatory
		ARRAY_COMPRESS_NORMAL=1<<(ARRAY_NORMAL+ARRAY_COMPRESS_BASE),
		ARRAY_COMPRESS_TANGENT=1<<(ARRAY_TANGENT+ARRAY_COMPRESS_BASE),
		ARRAY_COMPRESS_COLOR=1<<(ARRAY_COLOR+ARRAY_COMPRESS_BASE),
		ARRAY_COMPRESS_TEX_UV=1<<(ARRAY_TEX_UV+ARRAY_COMPRESS_BASE),
		ARRAY_COMPRESS_TEX_UV2=1<<(ARRAY_TEX_UV2+ARRAY_COMPRESS_BASE),
		ARRAY_COMPRESS_BONES=1<<(ARRAY_BONES+ARRAY_COMPRESS_BASE),
		ARRAY_COMPRESS_WEIGHTS=1<<(ARRAY_WEIGHTS+ARRAY_COMPRESS_BASE),
		ARRAY_COMPRESS_INDEX=1<<(ARRAY_INDEX+ARRAY_COMPRESS_BASE),

		ARRAY_FLAG_USE_2D_VERTICES=ARRAY_COMPRESS_INDEX<<1,
		ARRAY_FLAG_USE_16_BIT_BONES=ARRAY_COMPRESS_INDEX<<2,

		ARRAY_COMPRESS_DEFAULT=ARRAY_COMPRESS_VERTEX|ARRAY_COMPRESS_NORMAL|ARRAY_COMPRESS_TANGENT|ARRAY_COMPRESS_COLOR|ARRAY_COMPRESS_TEX_UV|ARRAY_COMPRESS_TEX_UV2|ARRAY_COMPRESS_WEIGHTS

	};


	enum PrimitiveType {
		PRIMITIVE_POINTS=0,
		PRIMITIVE_LINES=1,
		PRIMITIVE_LINE_STRIP=2,
		PRIMITIVE_LINE_LOOP=3,
		PRIMITIVE_TRIANGLES=4,
		PRIMITIVE_TRIANGLE_STRIP=5,
		PRIMITIVE_TRIANGLE_FAN=6,
		PRIMITIVE_MAX=7,
	};

	virtual RID mesh_create()=0;


	virtual void mesh_add_surface_from_arrays(RID p_mesh,PrimitiveType p_primitive,const Array& p_arrays,const Array& p_blend_shapes=Array(),uint32_t p_compress_format=ARRAY_COMPRESS_DEFAULT);
	virtual void mesh_add_surface(RID p_mesh,uint32_t p_format,PrimitiveType p_primitive,const DVector<uint8_t>& p_array,int p_vertex_count,const DVector<uint8_t>& p_index_array,int p_index_count,const AABB& p_aabb,const Vector<DVector<uint8_t> >& p_blend_shapes=Vector<DVector<uint8_t> >(),const Vector<AABB>& p_bone_aabbs=Vector<AABB>())=0;

	virtual void mesh_set_morph_target_count(RID p_mesh,int p_amount)=0;
	virtual int mesh_get_morph_target_count(RID p_mesh) const=0;

	enum MorphTargetMode {
		MORPH_MODE_NORMALIZED,
		MORPH_MODE_RELATIVE,
	};

	virtual void mesh_set_morph_target_mode(RID p_mesh,MorphTargetMode p_mode)=0;
	virtual MorphTargetMode mesh_get_morph_target_mode(RID p_mesh) const=0;

	virtual void mesh_surface_set_material(RID p_mesh, int p_surface, RID p_material)=0;
	virtual RID mesh_surface_get_material(RID p_mesh, int p_surface) const=0;

	virtual int mesh_surface_get_array_len(RID p_mesh, int p_surface) const=0;
	virtual int mesh_surface_get_array_index_len(RID p_mesh, int p_surface) const=0;

	virtual DVector<uint8_t> mesh_surface_get_array(RID p_mesh, int p_surface) const=0;
	virtual DVector<uint8_t> mesh_surface_get_index_array(RID p_mesh, int p_surface) const=0;

	virtual Array mesh_surface_get_arrays(RID p_mesh,int p_surface) const;

	virtual uint32_t mesh_surface_get_format(RID p_mesh, int p_surface) const=0;
	virtual PrimitiveType mesh_surface_get_primitive_type(RID p_mesh, int p_surface) const=0;

	virtual AABB mesh_surface_get_aabb(RID p_mesh, int p_surface) const=0;
	virtual Vector<DVector<uint8_t> > mesh_surface_get_blend_shapes(RID p_mesh, int p_surface) const=0;
	virtual Vector<AABB> mesh_surface_get_skeleton_aabb(RID p_mesh, int p_surface) const=0;

	virtual void mesh_remove_surface(RID p_mesh,int p_index)=0;
	virtual int mesh_get_surface_count(RID p_mesh) const=0;

	virtual void mesh_set_custom_aabb(RID p_mesh,const AABB& p_aabb)=0;
	virtual AABB mesh_get_custom_aabb(RID p_mesh) const=0;

	virtual void mesh_clear(RID p_mesh)=0;

	/* MULTIMESH API */


	virtual RID multimesh_create()=0;

	enum MultimeshTransformFormat {
		MULTIMESH_TRANSFORM_2D,
		MULTIMESH_TRANSFORM_3D,
	};

	enum MultimeshColorFormat {
		MULTIMESH_COLOR_NONE,
		MULTIMESH_COLOR_8BIT,
		MULTIMESH_COLOR_FLOAT,
	};

	virtual void multimesh_allocate(RID p_multimesh,int p_instances,MultimeshTransformFormat p_transform_format,MultimeshColorFormat p_color_format)=0;
	virtual int multimesh_get_instance_count(RID p_multimesh) const=0;

	virtual void multimesh_set_mesh(RID p_multimesh,RID p_mesh)=0;
	virtual void multimesh_instance_set_transform(RID p_multimesh,int p_index,const Transform& p_transform)=0;
	virtual void multimesh_instance_set_transform_2d(RID p_multimesh,int p_index,const Matrix32& p_transform)=0;
	virtual void multimesh_instance_set_color(RID p_multimesh,int p_index,const Color& p_color)=0;

	virtual RID multimesh_get_mesh(RID p_multimesh) const=0;
	virtual AABB multimesh_get_aabb(RID p_multimesh) const=0;

	virtual Transform multimesh_instance_get_transform(RID p_multimesh,int p_index) const=0;
	virtual Matrix32 multimesh_instance_get_transform_2d(RID p_multimesh,int p_index) const=0;
	virtual Color multimesh_instance_get_color(RID p_multimesh,int p_index) const=0;

	virtual void multimesh_set_visible_instances(RID p_multimesh,int p_visible)=0;
	virtual int multimesh_get_visible_instances(RID p_multimesh) const=0;


	/* IMMEDIATE API */

	virtual RID immediate_create()=0;
	virtual void immediate_begin(RID p_immediate,PrimitiveType p_rimitive,RID p_texture=RID())=0;
	virtual void immediate_vertex(RID p_immediate,const Vector3& p_vertex)=0;
	virtual void immediate_vertex_2d(RID p_immediate,const Vector3& p_vertex)=0;
	virtual void immediate_normal(RID p_immediate,const Vector3& p_normal)=0;
	virtual void immediate_tangent(RID p_immediate,const Plane& p_tangent)=0;
	virtual void immediate_color(RID p_immediate,const Color& p_color)=0;
	virtual void immediate_uv(RID p_immediate,const Vector2& tex_uv)=0;
	virtual void immediate_uv2(RID p_immediate,const Vector2& tex_uv)=0;
	virtual void immediate_end(RID p_immediate)=0;
	virtual void immediate_clear(RID p_immediate)=0;
	virtual void immediate_set_material(RID p_immediate,RID p_material)=0;
	virtual RID immediate_get_material(RID p_immediate) const=0;

	/* SKELETON API */

	virtual RID skeleton_create()=0;
	virtual void skeleton_allocate(RID p_skeleton,int p_bones,bool p_2d_skeleton=false)=0;
	virtual int skeleton_get_bone_count(RID p_skeleton) const=0;
	virtual void skeleton_bone_set_transform(RID p_skeleton,int p_bone, const Transform& p_transform)=0;
	virtual Transform skeleton_bone_get_transform(RID p_skeleton,int p_bone) const=0;
	virtual void skeleton_bone_set_transform_2d(RID p_skeleton,int p_bone, const Matrix32& p_transform)=0;
	virtual Matrix32 skeleton_bone_get_transform_2d(RID p_skeleton,int p_bone)const =0;

	/* Light API */

	enum LightType {
		LIGHT_DIRECTIONAL,
		LIGHT_OMNI,
		LIGHT_SPOT
	};

	enum LightParam {

		LIGHT_PARAM_ENERGY,
		LIGHT_PARAM_SPECULAR,
		LIGHT_PARAM_RANGE,
		LIGHT_PARAM_ATTENUATION,
		LIGHT_PARAM_SPOT_ANGLE,
		LIGHT_PARAM_SPOT_ATTENUATION,
		LIGHT_PARAM_SHADOW_MAX_DISTANCE,
		LIGHT_PARAM_SHADOW_SPLIT_1_OFFSET,
		LIGHT_PARAM_SHADOW_SPLIT_2_OFFSET,
		LIGHT_PARAM_SHADOW_SPLIT_3_OFFSET,
		LIGHT_PARAM_SHADOW_NORMAL_BIAS,
		LIGHT_PARAM_SHADOW_BIAS,
		LIGHT_PARAM_SHADOW_BIAS_SPLIT_SCALE,
		LIGHT_PARAM_MAX
	};

	virtual RID light_create(LightType p_type)=0;

	virtual void light_set_color(RID p_light,const Color& p_color)=0;
	virtual void light_set_param(RID p_light,LightParam p_param,float p_value)=0;
	virtual void light_set_shadow(RID p_light,bool p_enabled)=0;
	virtual void light_set_shadow_color(RID p_light,const Color& p_color)=0;
	virtual void light_set_projector(RID p_light,RID p_texture)=0;
	virtual void light_set_negative(RID p_light,bool p_enable)=0;
	virtual void light_set_cull_mask(RID p_light,uint32_t p_mask)=0;

	// omni light
	enum LightOmniShadowMode {
		LIGHT_OMNI_SHADOW_DUAL_PARABOLOID,
		LIGHT_OMNI_SHADOW_CUBE,
	};

	virtual void light_omni_set_shadow_mode(RID p_light,LightOmniShadowMode p_mode)=0;

	// omni light
	enum LightOmniShadowDetail {
		LIGHT_OMNI_SHADOW_DETAIL_VERTICAL,
		LIGHT_OMNI_SHADOW_DETAIL_HORIZONTAL
	};

	virtual void light_omni_set_shadow_detail(RID p_light,LightOmniShadowDetail p_detail)=0;

	// directional light
	enum LightDirectionalShadowMode {
		LIGHT_DIRECTIONAL_SHADOW_ORTHOGONAL,
		LIGHT_DIRECTIONAL_SHADOW_PARALLEL_2_SPLITS,
		LIGHT_DIRECTIONAL_SHADOW_PARALLEL_4_SPLITS
	};

	virtual void light_directional_set_shadow_mode(RID p_light,LightDirectionalShadowMode p_mode)=0;
	virtual void light_directional_set_blend_splits(RID p_light,bool p_enable)=0;

	/* PROBE API */

	virtual RID reflection_probe_create()=0;

	enum ReflectionProbeUpdateMode {
		REFLECTION_PROBE_UPDATE_ONCE,
		REFLECTION_PROBE_UPDATE_ALWAYS,
	};


	virtual void reflection_probe_set_update_mode(RID p_probe, ReflectionProbeUpdateMode p_mode)=0;
	virtual void reflection_probe_set_intensity(RID p_probe, float p_intensity)=0;
	virtual void reflection_probe_set_interior_ambient(RID p_probe, const Color& p_color)=0;
	virtual void reflection_probe_set_interior_ambient_energy(RID p_probe, float p_energy)=0;
	virtual void reflection_probe_set_interior_ambient_probe_contribution(RID p_probe, float p_contrib)=0;
	virtual void reflection_probe_set_max_distance(RID p_probe, float p_distance)=0;
	virtual void reflection_probe_set_extents(RID p_probe, const Vector3& p_extents)=0;
	virtual void reflection_probe_set_origin_offset(RID p_probe, const Vector3& p_offset)=0;
	virtual void reflection_probe_set_as_interior(RID p_probe, bool p_enable)=0;
	virtual void reflection_probe_set_enable_box_projection(RID p_probe, bool p_enable)=0;
	virtual void reflection_probe_set_enable_shadows(RID p_probe, bool p_enable)=0;
	virtual void reflection_probe_set_cull_mask(RID p_probe, uint32_t p_layers)=0;


	/* ROOM API */

	virtual RID room_create()=0;
	virtual void room_add_bounds(RID p_room, const DVector<Vector2>& p_convex_polygon,float p_height,const Transform& p_transform)=0;
	virtual void room_clear_bounds(RID p_room)=0;

	/* PORTAL API */

	// portals are only (x/y) points, forming a convex shape, which its clockwise
	// order points outside. (z is 0);

	virtual RID portal_create()=0;
	virtual void portal_set_shape(RID p_portal, const Vector<Point2>& p_shape)=0;
	virtual void portal_set_enabled(RID p_portal, bool p_enabled)=0;
	virtual void portal_set_disable_distance(RID p_portal, float p_distance)=0;
	virtual void portal_set_disabled_color(RID p_portal, const Color& p_color)=0;

	/* BAKED LIGHT API */

	/* CAMERA API */

	virtual RID camera_create()=0;
	virtual void camera_set_perspective(RID p_camera,float p_fovy_degrees, float p_z_near, float p_z_far)=0;
	virtual void camera_set_orthogonal(RID p_camera,float p_size, float p_z_near, float p_z_far)=0;
	virtual void camera_set_transform(RID p_camera,const Transform& p_transform)=0;
	virtual void camera_set_cull_mask(RID p_camera,uint32_t p_layers)=0;
	virtual void camera_set_environment(RID p_camera,RID p_env)=0;
	virtual void camera_set_use_vertical_aspect(RID p_camera,bool p_enable)=0;


	/* VIEWPORT TARGET API */

	virtual RID viewport_create()=0;

	virtual void viewport_set_size(RID p_viewport,int p_width,int p_height)=0;
	virtual void viewport_set_active(RID p_viewport,bool p_active)=0;
	virtual void viewport_set_parent_viewport(RID p_viewport,RID p_parent_viewport)=0;

	virtual void viewport_attach_to_screen(RID p_viewport,const Rect2& p_rect=Rect2(),int p_screen=0)=0;
	virtual void viewport_detach(RID p_viewport)=0;

	enum ViewportUpdateMode {
		VIEWPORT_UPDATE_DISABLED,
		VIEWPORT_UPDATE_ONCE, //then goes to disabled, must be manually updated
		VIEWPORT_UPDATE_WHEN_VISIBLE, // default
		VIEWPORT_UPDATE_ALWAYS
	};


	virtual void viewport_set_update_mode(RID p_viewport,ViewportUpdateMode p_mode)=0;
	virtual void viewport_set_vflip(RID p_viewport,bool p_enable)=0;

	enum ViewportClearMode {

		VIEWPORT_CLEAR_ALWAYS,
		VIEWPORT_CLEAR_NEVER,
		VIEWPORT_CLEAR_ONLY_NEXT_FRAME
	};

	virtual void viewport_set_clear_mode(RID p_viewport,ViewportClearMode p_clear_mode)=0;

	virtual RID viewport_get_texture(RID p_viewport) const=0;

	virtual void viewport_set_hide_scenario(RID p_viewport,bool p_hide)=0;
	virtual void viewport_set_hide_canvas(RID p_viewport,bool p_hide)=0;
	virtual void viewport_set_disable_environment(RID p_viewport,bool p_disable)=0;
	virtual void viewport_set_disable_3d(RID p_viewport,bool p_disable)=0;

	virtual void viewport_attach_camera(RID p_viewport,RID p_camera)=0;
	virtual void viewport_set_scenario(RID p_viewport,RID p_scenario)=0;
	virtual void viewport_attach_canvas(RID p_viewport,RID p_canvas)=0;
	virtual void viewport_remove_canvas(RID p_viewport,RID p_canvas)=0;
	virtual void viewport_set_canvas_transform(RID p_viewport,RID p_canvas,const Matrix32& p_offset)=0;
	virtual void viewport_set_transparent_background(RID p_viewport,bool p_enabled)=0;

	virtual void viewport_set_global_canvas_transform(RID p_viewport,const Matrix32& p_transform)=0;
	virtual void viewport_set_canvas_layer(RID p_viewport,RID p_canvas,int p_layer)=0;

	virtual void viewport_set_shadow_atlas_size(RID p_viewport,int p_size)=0;
	virtual void viewport_set_shadow_atlas_quadrant_subdivision(RID p_viewport,int p_quadrant,int p_subdiv)=0;


	/* ENVIRONMENT API */

	virtual RID environment_create()=0;

	enum EnvironmentBG {

		ENV_BG_CLEAR_COLOR,
		ENV_BG_COLOR,
		ENV_BG_SKYBOX,
		ENV_BG_CANVAS,
		ENV_BG_KEEP,
		ENV_BG_MAX
	};

	virtual void environment_set_background(RID p_env,EnvironmentBG p_bg)=0;
	virtual void environment_set_skybox(RID p_env,RID p_skybox)=0;
	virtual void environment_set_skybox_scale(RID p_env,float p_scale)=0;
	virtual void environment_set_bg_color(RID p_env,const Color& p_color)=0;
	virtual void environment_set_bg_energy(RID p_env,float p_energy)=0;
	virtual void environment_set_canvas_max_layer(RID p_env,int p_max_layer)=0;
	virtual void environment_set_ambient_light(RID p_env,const Color& p_color,float p_energy=1.0,float p_skybox_contribution=0.0)=0;

	//set default SSAO options
	//set default SSR options
	//set default SSSSS options

	enum EnvironmentGlowBlendMode {
		GLOW_BLEND_MODE_ADDITIVE,
		GLOW_BLEND_MODE_SCREEN,
		GLOW_BLEND_MODE_SOFTLIGHT,
		GLOW_BLEND_MODE_DISABLED,
	};
	virtual void environment_set_glow(RID p_env,bool p_enable,int p_radius,float p_intensity,float p_strength,float p_bloom_treshold,EnvironmentGlowBlendMode p_blend_mode)=0;
	virtual void environment_set_fog(RID p_env,bool p_enable,float p_begin,float p_end,RID p_gradient_texture)=0;


	enum EnvironmentToneMapper {
		ENV_TONE_MAPPER_LINEAR,
		ENV_TONE_MAPPER_LOG,
		ENV_TONE_MAPPER_REINHARDT,
		ENV_TONE_MAPPER_FILMIC,
		ENV_TONE_MAPPER_ACES_FILMIC
	};

	virtual void environment_set_tonemap(RID p_env,bool p_enable,float p_exposure,float p_white,float p_min_luminance,float p_max_luminance,float p_auto_exp_speed,float p_auto_exp_scale,EnvironmentToneMapper p_tone_mapper)=0;
	virtual void environment_set_adjustment(RID p_env,bool p_enable,float p_brightness,float p_contrast,float p_saturation,RID p_ramp)=0;


	/* SCENARIO API */


	virtual RID scenario_create()=0;

	enum ScenarioDebugMode {
		SCENARIO_DEBUG_DISABLED,
		SCENARIO_DEBUG_WIREFRAME,
		SCENARIO_DEBUG_OVERDRAW,
		SCENARIO_DEBUG_SHADELESS,

	};


	virtual void scenario_set_debug(RID p_scenario,ScenarioDebugMode p_debug_mode)=0;
	virtual void scenario_set_environment(RID p_scenario, RID p_environment)=0;
	virtual void scenario_set_reflection_atlas_size(RID p_scenario, int p_size,int p_subdiv)=0;
	virtual void scenario_set_fallback_environment(RID p_scenario, RID p_environment)=0;


	/* INSTANCING API */

	enum InstanceType {

		INSTANCE_NONE,
		INSTANCE_MESH,
		INSTANCE_MULTIMESH,
		INSTANCE_IMMEDIATE,
		INSTANCE_LIGHT,
		INSTANCE_REFLECTION_PROBE,
		INSTANCE_ROOM,
		INSTANCE_PORTAL,
		INSTANCE_MAX,
		/*INSTANCE_BAKED_LIGHT,
		INSTANCE_BAKED_LIGHT_SAMPLER,*/

		INSTANCE_GEOMETRY_MASK=(1<<INSTANCE_MESH)|(1<<INSTANCE_MULTIMESH)|(1<<INSTANCE_IMMEDIATE)
	};



	virtual RID instance_create2(RID p_base, RID p_scenario);

//	virtual RID instance_create(RID p_base,RID p_scenario)=0; // from can be mesh, light,  area and portal so far.
	virtual RID instance_create()=0; // from can be mesh, light, poly, area and portal so far.

	virtual void instance_set_base(RID p_instance, RID p_base)=0; // from can be mesh, light, poly, area and portal so far.
	virtual void instance_set_scenario(RID p_instance, RID p_scenario)=0; // from can be mesh, light, poly, area and portal so far.
	virtual void instance_set_layer_mask(RID p_instance, uint32_t p_mask)=0;
	virtual void instance_set_transform(RID p_instance, const Transform& p_transform)=0;
	virtual void instance_attach_object_instance_ID(RID p_instance,ObjectID p_ID)=0;
	virtual void instance_set_morph_target_weight(RID p_instance,int p_shape, float p_weight)=0;
	virtual void instance_set_surface_material(RID p_instance,int p_surface, RID p_material)=0;

	virtual void instance_attach_skeleton(RID p_instance,RID p_skeleton)=0;
	virtual void instance_set_exterior( RID p_instance, bool p_enabled )=0;
	virtual void instance_set_room( RID p_instance, RID p_room )=0;

	virtual void instance_set_extra_visibility_margin( RID p_instance, real_t p_margin )=0;

	// don't use these in a game!
	virtual Vector<ObjectID> instances_cull_aabb(const AABB& p_aabb, RID p_scenario=RID()) const=0;
	virtual Vector<ObjectID> instances_cull_ray(const Vector3& p_from, const Vector3& p_to, RID p_scenario=RID()) const=0;
	virtual Vector<ObjectID> instances_cull_convex(const Vector<Plane>& p_convex, RID p_scenario=RID()) const=0;

	enum InstanceFlags {
		INSTANCE_FLAG_VISIBLE,
		INSTANCE_FLAG_BILLBOARD,
		INSTANCE_FLAG_BILLBOARD_FIX_Y,
		INSTANCE_FLAG_CAST_SHADOW,
		INSTANCE_FLAG_DEPH_SCALE,
		INSTANCE_FLAG_VISIBLE_IN_ALL_ROOMS,
		INSTANCE_FLAG_USE_BAKED_LIGHT,
		INSTANCE_FLAG_MAX
	};

	enum ShadowCastingSetting {
		SHADOW_CASTING_SETTING_OFF,
		SHADOW_CASTING_SETTING_ON,
		SHADOW_CASTING_SETTING_DOUBLE_SIDED,
		SHADOW_CASTING_SETTING_SHADOWS_ONLY,
	};

	virtual void instance_geometry_set_flag(RID p_instance,InstanceFlags p_flags,bool p_enabled)=0;
	virtual void instance_geometry_set_cast_shadows_setting(RID p_instance, ShadowCastingSetting p_shadow_casting_setting) = 0;
	virtual void instance_geometry_set_material_override(RID p_instance, RID p_material)=0;


	virtual void instance_geometry_set_draw_range(RID p_instance,float p_min,float p_max,float p_min_margin,float p_max_margin)=0;
	virtual void instance_geometry_set_as_instance_lod(RID p_instance,RID p_as_lod_of_instance)=0;

	/* CANVAS (2D) */

	virtual RID canvas_create()=0;
	virtual void canvas_set_item_mirroring(RID p_canvas,RID p_item,const Point2& p_mirroring)=0;
	virtual void canvas_set_modulate(RID p_canvas,const Color& p_color)=0;


	virtual RID canvas_item_create()=0;
	virtual void canvas_item_set_parent(RID p_item,RID p_parent)=0;

	virtual void canvas_item_set_visible(RID p_item,bool p_visible)=0;
	virtual void canvas_item_set_light_mask(RID p_item,int p_mask)=0;

	virtual void canvas_item_set_transform(RID p_item, const Matrix32& p_transform)=0;
	virtual void canvas_item_set_clip(RID p_item, bool p_clip)=0;
	virtual void canvas_item_set_distance_field_mode(RID p_item, bool p_enable)=0;
	virtual void canvas_item_set_custom_rect(RID p_item, bool p_custom_rect,const Rect2& p_rect=Rect2())=0;
	virtual void canvas_item_set_modulate(RID p_item, const Color& p_color)=0;
	virtual void canvas_item_set_self_modulate(RID p_item, const Color& p_color)=0;

	virtual void canvas_item_set_draw_behind_parent(RID p_item, bool p_enable)=0;

	enum NinePatchAxisMode {
		NINE_PATCH_STRETCH,
		NINE_PATCH_TILE,
		NINE_PATCH_TILE_FIT,
	};

	virtual void canvas_item_add_line(RID p_item, const Point2& p_from, const Point2& p_to,const Color& p_color,float p_width=1.0,bool p_antialiased=false)=0;
	virtual void canvas_item_add_rect(RID p_item, const Rect2& p_rect, const Color& p_color)=0;
	virtual void canvas_item_add_circle(RID p_item, const Point2& p_pos, float p_radius,const Color& p_color)=0;
	virtual void canvas_item_add_texture_rect(RID p_item, const Rect2& p_rect, RID p_texture,bool p_tile=false,const Color& p_modulate=Color(1,1,1),bool p_transpose=false)=0;
	virtual void canvas_item_add_texture_rect_region(RID p_item, const Rect2& p_rect, RID p_texture,const Rect2& p_src_rect,const Color& p_modulate=Color(1,1,1),bool p_transpose=false)=0;
	virtual void canvas_item_add_nine_patch(RID p_item, const Rect2& p_rect, const Rect2& p_source, RID p_texture,const Vector2& p_topleft, const Vector2& p_bottomright,NinePatchAxisMode p_x_axis_mode=NINE_PATCH_STRETCH, NinePatchAxisMode p_y_axis_mode=NINE_PATCH_STRETCH,bool p_draw_center=true,const Color& p_modulate=Color(1,1,1))=0;
	virtual void canvas_item_add_primitive(RID p_item, const Vector<Point2>& p_points, const Vector<Color>& p_colors,const Vector<Point2>& p_uvs, RID p_texture,float p_width=1.0)=0;
	virtual void canvas_item_add_polygon(RID p_item, const Vector<Point2>& p_points, const Vector<Color>& p_colors,const Vector<Point2>& p_uvs=Vector<Point2>(), RID p_texture=RID())=0;
	virtual void canvas_item_add_triangle_array(RID p_item, const Vector<int>& p_indices, const Vector<Point2>& p_points, const Vector<Color>& p_colors,const Vector<Point2>& p_uvs=Vector<Point2>(), RID p_texture=RID(), int p_count=-1)=0;
	virtual void canvas_item_add_mesh(RID p_item, const RID& p_mesh,RID p_skeleton=RID())=0;
	virtual void canvas_item_add_multimesh(RID p_item, RID p_mesh,RID p_skeleton=RID())=0;
	virtual void canvas_item_add_set_transform(RID p_item,const Matrix32& p_transform)=0;
	virtual void canvas_item_add_clip_ignore(RID p_item, bool p_ignore)=0;
	virtual void canvas_item_set_sort_children_by_y(RID p_item, bool p_enable)=0;
	virtual void canvas_item_set_z(RID p_item, int p_z)=0;
	virtual void canvas_item_set_z_as_relative_to_parent(RID p_item, bool p_enable)=0;
	virtual void canvas_item_set_copy_to_backbuffer(RID p_item, bool p_enable,const Rect2& p_rect)=0;

	virtual void canvas_item_clear(RID p_item)=0;
	virtual void canvas_item_set_draw_index(RID p_item,int p_index)=0;

	virtual void canvas_item_set_material(RID p_item, RID p_material)=0;

	virtual void canvas_item_set_use_parent_material(RID p_item, bool p_enable)=0;

	virtual RID canvas_light_create()=0;
	virtual void canvas_light_attach_to_canvas(RID p_light,RID p_canvas)=0;
	virtual void canvas_light_set_enabled(RID p_light, bool p_enabled)=0;
	virtual void canvas_light_set_scale(RID p_light, float p_scale)=0;
	virtual void canvas_light_set_transform(RID p_light, const Matrix32& p_transform)=0;
	virtual void canvas_light_set_texture(RID p_light, RID p_texture)=0;
	virtual void canvas_light_set_texture_offset(RID p_light, const Vector2& p_offset)=0;
	virtual void canvas_light_set_color(RID p_light, const Color& p_color)=0;
	virtual void canvas_light_set_height(RID p_light, float p_height)=0;
	virtual void canvas_light_set_energy(RID p_light, float p_energy)=0;
	virtual void canvas_light_set_z_range(RID p_light, int p_min_z,int p_max_z)=0;
	virtual void canvas_light_set_layer_range(RID p_light, int p_min_layer,int p_max_layer)=0;
	virtual void canvas_light_set_item_cull_mask(RID p_light, int p_mask)=0;
	virtual void canvas_light_set_item_shadow_cull_mask(RID p_light, int p_mask)=0;

	enum CanvasLightMode {
		CANVAS_LIGHT_MODE_ADD,
		CANVAS_LIGHT_MODE_SUB,
		CANVAS_LIGHT_MODE_MIX,
		CANVAS_LIGHT_MODE_MASK,
	};

	virtual void canvas_light_set_mode(RID p_light, CanvasLightMode p_mode)=0;


	enum CanvasLightShadowFilter {
		CANVAS_LIGHT_FILTER_NONE,
		CANVAS_LIGHT_FILTER_PCF3,
		CANVAS_LIGHT_FILTER_PCF5,
		CANVAS_LIGHT_FILTER_PCF9,
		CANVAS_LIGHT_FILTER_PCF13,
	};

	virtual void canvas_light_set_shadow_enabled(RID p_light, bool p_enabled)=0;
	virtual void canvas_light_set_shadow_buffer_size(RID p_light, int p_size)=0;
	virtual void canvas_light_set_shadow_gradient_length(RID p_light, float p_length)=0;
	virtual void canvas_light_set_shadow_filter(RID p_light, CanvasLightShadowFilter p_filter)=0;
	virtual void canvas_light_set_shadow_color(RID p_light, const Color& p_color)=0;



	virtual RID canvas_light_occluder_create()=0;
	virtual void canvas_light_occluder_attach_to_canvas(RID p_occluder,RID p_canvas)=0;
	virtual void canvas_light_occluder_set_enabled(RID p_occluder,bool p_enabled)=0;
	virtual void canvas_light_occluder_set_polygon(RID p_occluder,RID p_polygon)=0;
	virtual void canvas_light_occluder_set_transform(RID p_occluder,const Matrix32& p_xform)=0;
	virtual void canvas_light_occluder_set_light_mask(RID p_occluder,int p_mask)=0;

	virtual RID canvas_occluder_polygon_create()=0;
	virtual void canvas_occluder_polygon_set_shape(RID p_occluder_polygon,const DVector<Vector2>& p_shape,bool p_closed)=0;
	virtual void canvas_occluder_polygon_set_shape_as_lines(RID p_occluder_polygon,const DVector<Vector2>& p_shape)=0;

	enum CanvasOccluderPolygonCullMode {
		CANVAS_OCCLUDER_POLYGON_CULL_DISABLED,
		CANVAS_OCCLUDER_POLYGON_CULL_CLOCKWISE,
		CANVAS_OCCLUDER_POLYGON_CULL_COUNTER_CLOCKWISE,
	};
	virtual void canvas_occluder_polygon_set_cull_mode(RID p_occluder_polygon,CanvasOccluderPolygonCullMode p_mode)=0;


	/* CURSOR */
	virtual void cursor_set_rotation(float p_rotation, int p_cursor = 0)=0; // radians
	virtual void cursor_set_texture(RID p_texture, const Point2 &p_center_offset = Point2(0, 0), int p_cursor=0, const Rect2 &p_region=Rect2())=0;
	virtual void cursor_set_visible(bool p_visible, int p_cursor = 0)=0;
	virtual void cursor_set_pos(const Point2& p_pos, int p_cursor = 0)=0;

	/* BLACK BARS */


	virtual void black_bars_set_margins(int p_left, int p_top, int p_right, int p_bottom)=0;
	virtual void black_bars_set_images(RID p_left, RID p_top, RID p_right, RID p_bottom)=0;


	/* FREE */

	virtual void free( RID p_rid )=0; ///< free RIDs associated with the visual server

	/* EVENT QUEUING */

	virtual void draw()=0;
	virtual void sync()=0;
	virtual bool has_changed() const=0;
	virtual void init()=0;
	virtual void finish()=0;

	/* STATUS INFORMATION */

	enum RenderInfo {

		INFO_OBJECTS_IN_FRAME,
		INFO_VERTICES_IN_FRAME,
		INFO_MATERIAL_CHANGES_IN_FRAME,
		INFO_SHADER_CHANGES_IN_FRAME,
		INFO_SURFACE_CHANGES_IN_FRAME,
		INFO_DRAW_CALLS_IN_FRAME,
		INFO_USAGE_VIDEO_MEM_TOTAL,
		INFO_VIDEO_MEM_USED,
		INFO_TEXTURE_MEM_USED,
		INFO_VERTEX_MEM_USED,
	};

	virtual int get_render_info(RenderInfo p_info)=0;


	/* Materials for 2D on 3D */


	RID material_2d_get(bool p_shaded, bool p_transparent, bool p_cut_alpha,bool p_opaque_prepass);

	/* TESTING */

	virtual RID get_test_cube()=0;

	virtual RID get_test_texture();
	virtual RID get_white_texture();

	virtual RID make_sphere_mesh(int p_lats,int p_lons,float p_radius);

	virtual void mesh_add_surface_from_mesh_data( RID p_mesh, const Geometry::MeshData& p_mesh_data);
	virtual void mesh_add_surface_from_planes( RID p_mesh, const DVector<Plane>& p_planes);

	virtual void set_boot_image(const Image& p_image, const Color& p_color,bool p_scale)=0;
	virtual void set_default_clear_color(const Color& p_color)=0;

	enum Features {
		FEATURE_SHADERS,
		FEATURE_MULTITHREADED,
	};

	virtual bool has_feature(Features p_feature) const=0;

	VisualServer();
	virtual ~VisualServer();

};

// make variant understand the enums

VARIANT_ENUM_CAST( VisualServer::CubeMapSide );
VARIANT_ENUM_CAST( VisualServer::TextureFlags );
VARIANT_ENUM_CAST( VisualServer::ShaderMode );
VARIANT_ENUM_CAST( VisualServer::ArrayType );
VARIANT_ENUM_CAST( VisualServer::ArrayFormat );
VARIANT_ENUM_CAST( VisualServer::PrimitiveType );
VARIANT_ENUM_CAST( VisualServer::LightType );
VARIANT_ENUM_CAST( VisualServer::LightParam );
VARIANT_ENUM_CAST( VisualServer::ScenarioDebugMode );
VARIANT_ENUM_CAST( VisualServer::InstanceType );
VARIANT_ENUM_CAST( VisualServer::RenderInfo );

//typedef VisualServer VS; // makes it easier to use
#define VS VisualServer

#endif
