# Layer Node JSON Format

- [Example](#example)
- [See basic needed types here](#json-basic-types)

```JSON
{
    "Type": "Layer",
    "Data": {
        "Flyable": __flyable, // <LayerDataPropertyType>
        "Movable": __movable, // <LayerDataPropertyType>
        "Node": __node // <LayerDataPropertyType>
    }
}
```

## JSON Basic Types 

```JSON
<LayerDataPropertyType> = {
    "Id": __id,
    "ParentId": __parent_id,
    "Text": __text,
    "Order": __order,
    "Children": [
        <LayerDataPropertyType>,
        <LayerDataPropertyType>,
        ...
    ]
}
```

## Example

```JSON
{
    "Type": "Layer",
    "Data": {
        "Flyable": {
            "Id": 100,
            "ParentId": 0,
            "Text": "Flyable",
            "Order": 1,
            "Children": [
                {
                    "Id": 101,
                    "ParentId": 100,
                    "Text": "Flyable",
                    "Order": 1,
                    "Children": [
                        {
                            "Id": 102,
                            "ParentId": 101,
                            "Text": "Unknown",
                            "Order": 1
                        },
                        {
                            "Id": 103,
                            "ParentId": 101,
                            "Text": "SpeedUp200",
                            "Order": 2
                        },
                        {
                            "Id": 104,
                            "ParentId": 101,
                            "Text": "SpeedDown200",
                            "Order": 3
                        }
                    ]
                },
                {
                    "Id": 105,
                    "ParentId": 100,
                    "Text": "Route",
                    "Order": 2
                },
                {
                    "Id": 106,
                    "ParentId": 100,
                    "Text": "Status",
                    "Order": 3
                }
            ]
        },
        "Movable": {
            "Id": 200,
            "ParentId": 0,
            "Text": "Movable",
            "Order": 1,
            "Children": [
                {
                    "Id": 201,
                    "ParentId": 200,
                    "Text": "Movable",
                    "Order": 1,
                    "Children": [
                        {
                            "Id": 202,
                            "ParentId": 201,
                            "Text": "Unknown",
                            "Order": 1
                        },
                        {
                            "Id": 203,
                            "ParentId": 201,
                            "Text": "SpeedUp200",
                            "Order": 2
                        },
                        {
                            "Id": 204,
                            "ParentId": 201,
                            "Text": "SpeedDown200",
                            "Order": 3
                        }
                    ]
                },
                {
                    "Id": 205,
                    "ParentId": 200,
                    "Text": "Route",
                    "Order": 2
                },
                {
                    "Id": 206,
                    "ParentId": 200,
                    "Text": "Status",
                    "Order": 3
                }
            ]
        },
        "Node": {
            "Id": 300,
            "ParentId": 0,
            "Text": "Node",
            "Order": 3,
            "Children": [
                {
                    "Id": 301,
                    "ParentId": 300,
                    "Text": "Node",
                    "Order": 1,
                    "Children": [
                        {
                            "Id": 302,
                            "ParentId": 301,
                            "Text": "Active",
                            "Order": 1
                        },
                        {
                            "Id": 303,
                            "ParentId": 301,
                            "Text": "Inactive",
                            "Order": 2
                        }
                    ]
                },
                {
                    "Id": 304,
                    "ParentId": 300,
                    "Text": "Circle",
                    "Order": 2
                },
                {
                    "Id": 305,
                    "ParentId": 300,
                    "Text": "Status",
                    "Order": 3
                },
                {
                    "Id": 306,
                    "ParentId": 300,
                    "Text": "Polygon",
                    "Order": 4
                }
            ]
        }
    }
}
```



