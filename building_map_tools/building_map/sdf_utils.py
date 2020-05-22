import yaml
from xml.etree.ElementTree import ElementTree, Element, SubElement

def generate_box(size):
    '''size: [x, y, z]'''
    [x, y, z] = size
    box_ele = Element('box')
    size_ele = SubElement(box_ele, 'size')
    size_ele.text = f'{x} {y} {z}'

    return box_ele

def generate_collide_bitmask(bitmask):
    surface = Element('surface')
    contact = SubElement(surface, 'contact')
    collide_bitmask = SubElement(contact, 'collide_bitmask')
    collide_bitmask.text = '{}'.format(bitmask)

    return surface

def generate_visual(name, pose, size, material=None):
    visual_ele = Element('visual')
    visual_ele.set('name', name)

    if pose is not None:
        visual_ele.append(pose)

    visual_geometry_ele = SubElement(visual_ele, 'geometry')
    visual_geometry_ele.append(generate_box(size))

    if material:
        visual_ele.append(material)

    return visual_ele

def generate_collision(name, pose, size, bitmask=None):
    collision_ele = Element('collision')
    collision_ele.set('name', name)

    if pose is not None:
        collision_ele.append(pose)

    collision_geometry_ele = SubElement(collision_ele, 'geometry')
    collision_geometry_ele.append(generate_box(size))

    if bitmask:
        collision_ele.append(generate_collide_bitmask(bitmask))

    return collision_ele

def generate_box_link(name, size, pose, visual=True, collision=True, material=None, bitmask=None):
    link = Element('link')
    link.set('name', name)
    link.append(pose)

    if visual:
        link.append(generate_visual(name + '_visual', None, size))
    if collision:
        link.append(generate_collision(name + '_collision', None, size, bitmask))

    return link

def generate_joint(joint_name, joint_type, parent_link, child_link, joint_axis='z',
                       lower_limit=None, upper_limit=None, max_effort=None, pose=None):
    joint = Element('joint')
    joint.set('name', joint_name)

    supported_joint_types = ['fixed', 'prismatic', 'revolute']
    if joint_type not in supported_joint_types:
        raise RuntimeError('joint type {} not supported.'.format(joint_type))
    joint.set('type', joint_type)

    parent = SubElement(joint, 'parent')
    parent.text = parent_link

    child = SubElement(joint, 'child')
    child.text = child_link

    if joint_type == 'fixed':
        return joint

    axis = SubElement(joint, 'axis')
    xyz = SubElement(axis, 'xyz')
    if joint_axis == 'x':
        xyz.text = '1 0 0'
    elif joint_axis == 'y':
        xyz.text = '0 1 0'
    elif joint_axis == 'z':
        xyz.text = '0 0 1'
    else:
        raise RuntimeError('Axis requested is undefined, only "x", "y" and "z" available')

    if lower_limit is not None and upper_limit is not None:
        limit = SubElement(axis, 'limit')
        lower = SubElement(limit, 'lower')
        lower.text = '{}'.format(lower_limit)
        upper = SubElement(limit, 'upper')
        upper.text = '{}'.format(upper_limit)

        if max_effort is not None:
            effort = SubElement(limit, 'effort')
            effort.text = '{}'.format(max_effort)

    if pose is not None:
        joint.append(pose)

    return joint