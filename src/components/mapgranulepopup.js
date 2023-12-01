import { TbArrowRight } from "react-icons/tb";
import { Popup } from "react-leaflet";
import { Link } from "react-router-dom";

export default function GranulePopup({ granule }) {
    return (
        <Popup>
            <h1 className="font-bold mb-1">Granula #{granule.granuleNumber}</h1>
            <Link to={`/granule/${granule.granuleNumber - 1}/details`} className="btn-ghost">Afișeză detalii <TbArrowRight className="inline-block" /></Link>
        </Popup>
    )
}