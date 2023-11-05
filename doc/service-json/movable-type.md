# Tempalte

- [Example](#example)
- [See basic needed types here](#json-basic-types)

```JSON
{
    "Type": "Movable",
    "Data": { // all `values` can be a simple type or <CategorizedPropertyType> object
        "Name": __name,
        "Id": __id,
        "Color": __color,
        "Url2d": __url2d,
        "Url3d": __url3d,
        "ImgSrc": __image_source,
        "Longitude": __longitude, 
        "Latitude": __latitude, 
        "Altitude": __altitude, 
        "Heading": __heading, 
        "Speed": __speed, 
        "LayersId": [
            __layer1id,
            __layer2id,
            ...
        ]
    }
}
```

## JSON Basic Types 

```JSON
<CategorizedPropertyType> = {
    "value": __value,
    "category": __category
}
```

## Example

```JSON
{
    "Type": "Movable",
    "Data": {
        "Name": {
            "value": "Movable1",
            "category": "Main Information"
        },
        "Id": {
            "value": 40001,
            "category": "Location Information"
        },
        "Color": "#aabbcc",
        "Url2d": "../data/models/car/car.png",
        "Url3d": "../data/models/car/car.osgb",
        "ImgSrc": "qrc:/Resources/station.jpg",
        "Longitude": {
            "value": 33.3,
            "category": "Location Information"
        }, 
        "Latitude": {
            "value": 11.1,
            "category": "Location Information"
        }, 
        "Altitude": {
            "value": 22.2,
            "category": "Location Information"
        }, 
        "Heading": {
            "value": 44.44,
            "category": "Location Information"
        }, 
        "Speed": {
            "value": 99.9,
            "category": "Location Information"
        }, 
        "LayersId": [
            1,
            2
        ]
    }
}
```