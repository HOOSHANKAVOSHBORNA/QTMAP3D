# LayerData

- [Example](#example)

# LayerData types

* id
* parentId
* text
* order
* command
* isComposite
* [children](#layerdata)

# Example

```JSON
{
 "Id": 120,
 "ParentId": 130,
 "Text": "some text",
 "Order": 5,
 "Command": "Update",
 "IsComposite": false,
 "children": [
    {
        "Id": 121,
        "ParentId": 130,
        "Text": "some text",
        "Order": 5,
        "Command": "Update",
        "IsComposite": false,
    }
 ]
}
```

