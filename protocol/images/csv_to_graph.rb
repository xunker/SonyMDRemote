#!/usr/bin/env ruby
# filename = "PRINT_00.CSV"
filename = "PRINT_01.CSV"
# filename = "PRINT_02.CSV"

# require 'gruff'

# g = Gruff::Line.new#(2000) # arg is width
# g.title = filename

# g.y_axis_label = 'Volts'
# g.x_axis_label = 'Time'

# g.hide_dots = true
# g.reference_lines = false
# g.graph_height = 100

# labels_x = []
# data_y = []

# number_of_points = 500
# File.open(filename) do |file|
#   while (!file.eof && number_of_points > 0)
#     values = file.readline.strip.split(',')
#     # skip label lines by looking for column values that don't contain numbers
#     next unless values.all?{|v| v =~ /\d/}
#     labels_x << values.first.to_f
#     data_y << values.last.to_f.round

#     number_of_points -= 1
#   end
# end

# g.dataxy(:read, labels_x, data_y, '#ff0000')
# g.write('exciting.png')

# -- mittsu
# export MITTSU_LIBGLFW_FILE=libglfw.3.dylib

require 'mittsu'

SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600
ASPECT = SCREEN_WIDTH.to_f / SCREEN_HEIGHT.to_f

scene = Mittsu::Scene.new
camera = Mittsu::PerspectiveCamera.new(50.0, ASPECT, 0.1, 1000.0)

renderer = Mittsu::OpenGLRenderer.new width: SCREEN_WIDTH, height: SCREEN_HEIGHT, title: '11 Continous Keyboard Example'

geometry = Mittsu::BoxGeometry.new(0.5, 0.5, 0.5)
material = Mittsu::MeshBasicMaterial.new(color: 0x00ff00)
# cube = Mittsu::Mesh.new(geometry, material)
# scene.add(cube)

# box_geometry = Mittsu::BoxGeometry.new(1.0, 1.0, 1.0)
# room_material = Mittsu::MeshPhongMaterial.new(color: 0xffffff)
# room_material.side = Mittsu::BackSide
# room = Mittsu::Mesh.new(box_geometry, room_material)
# room.scale.set(10.0, 10.0, 10.0)
# scene.add(room)


number_of_points = 100000


material = Mittsu::LineBasicMaterial.new(color: 0xff00ff)

camera_pos = []

File.open(filename) do |file|
  # prev_time = 0
  # prev_value = 0
  geometry = Mittsu::Geometry.new()
  # geometry.vertices.push(Mittsu::Vector3.new(prev_time, prev_value, 0.0))
  while (!file.eof && number_of_points > 0)
    values = file.readline.strip.split(',')
    # skip label lines by looking for column values that don't contain numbers
    next unless values.all?{|v| v =~ /\d/}

    if camera_pos.length == 0
      camera_pos = [values.first.to_f, values.last.to_f, 0]
    end

    geometry.vertices.push(Mittsu::Vector3.new(-values.first.to_f, values.last.to_f, 0.0))

    number_of_points -= 1
  end

  line = Mittsu::Line.new(geometry, material)
  scene.add(line)
end
camera.position.x = -camera_pos.first.to_f
camera.position.y = camera_pos[1].to_f/1
camera.position.z = -5
camera.rotation.y = 3.149

prev_mouse_x = nil
prev_mouse_y = nil

renderer.window.on_resize do |width, height|
  renderer.set_viewport(0, 0, width, height)
  camera.aspect = width.to_f / height.to_f
  camera.update_projection_matrix
end

renderer.window.run do

  if renderer.window.key_down?(GLFW_KEY_S) || renderer.window.key_down?(GLFW_KEY_W)
    cos = Math.cos(camera.rotation.y)
    sin = Math.sin(camera.rotation.y)
    z_val = 0.01 * cos
    x_val = 0.01 * sin
    if renderer.window.key_down?(GLFW_KEY_W)
      camera.position.z -= z_val
      camera.position.x -= x_val
    elsif renderer.window.key_down?(GLFW_KEY_S)
      camera.position.z += z_val
      camera.position.x += x_val
    end
    puts "Move: camera: rotation: x: #{camera.rotation.x}, y: #{camera.rotation.y}, z: #{camera.rotation.z}, position: x: #{camera.position.x}, y: #{camera.position.y}, z: #{camera.position.z}"
  end

  #strafe
  if renderer.window.key_down?(GLFW_KEY_E) || renderer.window.key_down?(GLFW_KEY_Q)
    cos = Math.cos(camera.rotation.y+(6.25/4))
    sin = Math.sin(camera.rotation.y+(6.25/4))

    z_val = 0.01 * cos
    x_val = 0.01 * sin
    if renderer.window.key_down?(GLFW_KEY_Q)
      camera.position.z -= z_val
      camera.position.x -= x_val
    elsif renderer.window.key_down?(GLFW_KEY_E)
      camera.position.z += z_val
      camera.position.x += x_val
    end
    puts "Strafe: #{camera.rotation.y}: #{sin} #{cos}, #{camera.position.z} #{camera.position.x}"
  end

  camera.rotation.y += 0.01 if renderer.window.key_down?(GLFW_KEY_A)
  camera.rotation.y -= 0.01 if renderer.window.key_down?(GLFW_KEY_D)
  camera.position.x += 0.1 if renderer.window.key_down?(GLFW_KEY_Z)
  camera.position.x -= 0.1 if renderer.window.key_down?(GLFW_KEY_C)


  # camera.rotation.y = -6.25 if camera.rotation.y > 6.25
  # camera.rotation.y = 6.25 if camera.rotation.y < -6.25
  camera.rotation.y = 0 if camera.rotation.y > 6.25
  camera.rotation.y = 0 if camera.rotation.y < -6.25


  if renderer.window.key_down?(GLFW_KEY_ENTER)
    camera.position.z = 5.0
    camera.position.x = 0.0
    camera.position.y = 0.0

    camera.rotation.z = 0.0
    camera.rotation.x = 0.0
    camera.rotation.y = 0.0
  end

  # puts camera.rotation.y
  # puts "#{camera.rotation.y}: #{Math.cos(camera.rotation.y)}"

  exit 1 if renderer.window.key_down?(GLFW_KEY_ESCAPE)

  renderer.render(scene, camera)
end

