# Service JSON

## General JSON Format

```JSON
{
    "Type": __your_type_name,
    "Data": __your_type_data // JSON object type
}
```

## Commands

* Add
* Update
* Remove

## Types

- [X] [Circle](./CircleData.md)
- [X] [Layer](./LayerData.md)
- [X] [Node](./NodeData.md)
- [X] [Polygon](./PolyLineData.md)
- [X] [Explosion](./ExplosionData.md)
- [X] [AssignmentState](./assignment.md)

## How to write new doc for service json

### Doc Template

> you should insert your names in `__name__` parts.

```md
# __Type_Name__ 

- [Example](#example)

__write_main_json_here__

## JSON Basic Types 

**No Basic Needed!**

## Example

__write_example_json_here__

```

## JSON Literals

- string: `"some string"`
- number: `2`, `1.34`, ...
- boolean: `true`, `false`
- null: `null`
- array: `[]`
- object: a `JSON` in your `JSON`
    > In Doc we should define our new types that are our object types. \
    > Value types are shown in comments. (if no comment value is string as default)


