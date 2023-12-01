import { TbInfoCircle, TbWorldStar } from "react-icons/tb";
import { Link } from "react-router-dom";

export default function MapNavbar({ children, location, centerClassName }) {
    return (
        <div className="navbar bg-base-100 border-b-4 border-base-200 fixed top-0">
            <div className="navbar-start"></div>
            <div className={`navbar-center ${centerClassName}`}>
                <Link to='/' className="btn btn-ghost normal-case font-bold text-xl"><TbWorldStar /> igeomap</Link>
            </div>
            <div className="navbar-end">
                <button className="btn btn-ghost btn-circle">
                    <div className="indicator" onClick={() => window.location = 'https://www.spaceappschallenge.org/2023/find-a-team/cnlr-web-development/?tab=project'}>
                        <TbInfoCircle size={23} />
                    </div>
                </button>
            </div>
        </div>
    )
}