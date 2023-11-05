# Status

- [Example](#example)
- [See basic needed types here](#json-basic-types)

```JSON
{
    "Type": "Status",
    "Data": {
        "Name": __name,
        "Id": __id,
        "Longitude": __longitude,
        "Latitude": __latitude,
        "Altitude": __altitude,
        "LayerId": __layer_id,
        "FieldData": {
            "Heading": __heading,
            "Speed": __speed
        }
    }
}
```

## JSON Basic Types 

**No Basic Needed!**

## Example

```JSON
{
    "Type": "Status",
    "Data": {
        "Name": "Node1",
        "Id": 30001,
        "Longitude": 4.2,
        "Latitude": 3.1,
        "Altitude": 4.2,
        "LayerId": 3,
        "FieldData": {
            "Heading": 2.1,
            "Speed": 90
        }
    }
}
```

